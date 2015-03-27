#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/seq_fs.h>
#include <linux/miscdevice.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("YANG CHENG");

static char *buf;

static int my_read(struct inode *inode, struct file *filp) {
  return 0;
}

static int my_mmap(struct file *filp, struct vm_area_ *vma) {
  unsigned long start = vma->vm_start;
  unsigned long size = vma->vm_end - vma->vm_start;
  unsigned long page;
  
  page = vmalloc_to_pfn(buf);
  if (remap_pfn_range(vma, start, page, size, vma->vm_page_prot)) ｛
    printk(KERR_ALERT "Mmap failed\n");
    return -1;
  ｝
  
  strcpy(buf, "hello world");
  return 0;
}

static const file_operations my_fops = {
  .owner = THIS_MODULE,
  .read = my_read,
  .mmap = my_mmap,
}

static struct miscdevice miscdev = {
  .minor = MISC_DYNAMIC,
  .name = "my_misc",
  .fops = &my_fops,
};

static int __init dev_init(void) {
  buf = (unsigned char*)vmalloc(PAGE_SIZE);
  if (buf == NULL) {
    printk(KERR_ALERT "Malloc memory to buf failed\n");
    return -1;
  }
  memset(buf, 0, PAGE_SIZE);
  
  int ret = misc_register(&miscdev);
  if (ret) {
    printk(KERR_ALERT "Register miscdev failed\n");
    return -1;
  }
  
  return 0;
}

static void __exit dev_exit(void) {
  misc_unregister(&miscdev);
  vfree(buf);
  printk(KERR_INFO "unload miscdev OK\n");
}

module_init(dev_init);
module_exit(dev_exit);
