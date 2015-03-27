#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YANG CHENG");

#define BUFF_SIZE 4096
#define ENTRY "my_entry"

int temp = 0;
char msg[BUFF_SIZE];
static struct proc_dir_entry *test;

static ssize_t proc_read(struct file *filp, char __user *buf,
                        size_t count, loff_t *off) {
  if (temp) {
    temp = 0;
    return 0;
  }

  temp = strlen(msg);
  if(copy_to_user(buf, msg, temp)) {
    printk(KERN_ALERT "Copy to user fault\n");
    return -EFAULT;
  }

  return temp;
}

static ssize_t proc_write(struct file *filp, const char __user *buf,
                          size_t count, loff_t *off) {
  if(copy_from_user(msg, buf, count)) {
    printk(KERN_ALERT "Copy from user fault\n");
    return -EFAULT;
  }

  msg[count] = 0;
  return count;
}

static const struct file_operations my_fops = {
  .owner = THIS_MODULE,
  .read = proc_read,
  .write = proc_write,
};

static int __init proc_init(void) {
  test = proc_create(ENTRY, 0666, NULL, &my_fops);
  if (!test) {
    printk(KERN_ERR "Create entry %s fault\n", ENTRY);
    return -1;
  }

  printk(KERN_INFO "Create entry, success");
  return 0;
}

static void __exit proc_exit(void) {
  remove_proc_entry(ENTRY, NULL);
  printk(KERN_INFO "Remove entry, success");
}

module_init(proc_init);
module_exit(proc_exit);
