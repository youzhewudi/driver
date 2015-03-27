#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("YANG CHENG");

static char *buf;

static int my_open(struct inode *inode, struct file *filp) {
  return 0;
}

static int my_mmap(struct file *filp, struct vm_area_struct *vma) {
  unsigned long start = vma->vm_start;
  unsigned long size = vma->vm_end - vma->vm_start;
  unsigned long page;

  page = vmalloc_to_pfn(buf);
  if (remap_pfn_range(vma, start, page, size, vma->vm_page_prot)) {
    printk(KERN_ALERT "Mmap failed\n");
    return -1;
  }

  strcpy(buf, "hello world");
  return 0;
}

static const struct file_operations my_fops = {
  .owner = THIS_MODULE,
  .open = my_open,
  .mmap = my_mmap,
};

static struct miscdevice miscdev = {
  .minor = MISC_DYNAMIC_MINOR,
  .name = "my_misc",
  .fops = &my_fops,
};

static int __init dev_init(void) {
  int ret;
  buf = (unsigned char*)vmalloc(PAGE_SIZE);
  if (buf == NULL) {
    printk(KERN_ALERT "Malloc memory to buf failed\n");
    return -1;
  }
  memset(buf, 0, PAGE_SIZE);


  ret = misc_register(&miscdev);
  if (ret) {
    printk(KERN_ALERT "Register miscdev failed\n");
    return -1;
  }

  return 0;
}

static void __exit dev_exit(void) {
  misc_deregister(&miscdev);
  vfree(buf);
  printk(KERN_INFO "unload miscdev OK\n");
}

module_init(dev_init);
module_exit(dev_exit);
