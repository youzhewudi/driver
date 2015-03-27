#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("YANG CHENG");

#define DIR  "mydir"
#define ENTRY "pool"

static struct proc_dir_entry *dir;
static struct proc_dir_entry *entry;

static int proc_show(struct seq_file *file, void *v) {
  seq_printf(file, "hello world\n");
  return 0;
}

static int proc_open(struct inode *inode, struct file *file) {
  return single_open(file, proc_show, NULL);
}

static struct file_operations my_fops = {
  .owner = THIS_MODULE,
  .open = proc_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

static int __init proc_init(void) {
  dir = proc_mkdir(DIR, NULL);
  if (NULL == dir) {
    printk(KERN_ALERT "Create dir %s fault\n", DIR);
    return -1;
  }

  entry = proc_create(ENTRY, 0666, dir, &my_fops);
  if (NULL == dir) {
    printk(KERN_ALERT "Create entry %s fault\n", ENTRY);
    remove_proc_entry(DIR, NULL);
    return -1;
  }

  return 0;
}

static void __exit proc_exit(void) {
  remove_proc_entry(ENTRY, dir);
  remove_proc_entry(DIR, NULL);
}

module_init(proc_init);
module_exit(proc_exit);
