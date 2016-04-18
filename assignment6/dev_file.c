#include <linux/init.h>
#include <linux/module.h>

/* printk() */
#include <linux/kernel.h> 

/* kmalloc() */
#include <linux/slab.h>

/* file_operations struct */
#include <linux/fs.h> 

/* size_t */
#include <linux/types.h> 

/* O_ACCMODE */
#include <linux/fcntl.h>

/* copy_from/to_user */
#include <asm/uaccess.h> 


/*The functions required to make the driver are:*/
int dev_file_init(void);
void dev_file_exit(void);
int dev_file_open(struct inode *inode, struct file *filp);
int dev_file_release(struct inode *inode, struct file *filp);
ssize_t dev_file_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t dev_file_write(struct file *filp, char *buf, size_t count, loff_t *f_pos);

/* Declaration of the init and exit functions */
module_init(dev_file_init);
module_exit(dev_file_exit);

/*Data structure for using file functions:*/
struct file_operations dev_file_fops = {
  read: dev_file_read,
  write: dev_file_write,
  open: dev_file_open,
  release: dev_file_release
}; 

/* Declare a buffer to store input characters */
char *kernel_buffer;
int count = 0;
int buffer_size = 128;

static int major_number = 60;

/*The functions below need to be completed as per the comments.*/

int dev_file_init(void) {
  int result;

  /* Registering device */
  result = register_chrdev(major_number, "dev_file", &dev_file_fops);

  /* Handle error case for result.i.e. if device was not registered successfully */
  if(result<0)
  {
    printk("Device registration failed\n");
    return -1;
  }

  if((kernel_buffer = kmalloc(buffer_size, GFP_KERNEL)) == NULL)
  {
    printk("kmalloc() failed\n");
    return -1;
  }

  printk("Device initialisation succesfull\n");
  return 0;
}

void dev_file_exit(void) {
  /* Unregistering device */
  unregister_chrdev(major_number, "dev_file");

  /* Free the buffer using kfree */

  kfree(kernel_buffer);

  printk("Device unregistration successful\n");
}

/* To handle open and release, maintain a count variable to check how many times the file
    has been opened. Increment for every open and decrement for every release */

/* This is called when any process tries to open the device file such as cat /dev/dev_file*/
int dev_file_open(struct inode *inode, struct file *filp) {

  /* Check if device is already open, if so return error*/
  if(count++==0)
  {
    printk("Device driver opened\n");
    return 0;
  }
  else
  {
    printk("Device driver already opened\n");
    return -1;
  }  
}

/* This is called when any process closes the device file */
int dev_file_release(struct inode *inode, struct file *filp) {

  count--;
 /* Close device */
  return 0;

}

ssize_t dev_file_read(struct file *filp, char *user_buffer,
                    size_t count, loff_t *f_pos) {
  int no_bytes_read;

  if(kernel_buffer==0)
  {
    return 0;
  }
  else if((*f_pos+strlen(kernel_buffer))>strlen(kernel_buffer))
  {
    printk("In read, returning 0\n");
    return 0;
  }

  /* Use copy_to_user() to transfer data to user space */
  no_bytes_read = copy_to_user(user_buffer, kernel_buffer, strlen(kernel_buffer));
  printk("no of bytes read %d\n", no_bytes_read);

  no_bytes_read = strlen(kernel_buffer) - no_bytes_read;

  *f_pos = *f_pos+strlen(kernel_buffer);

  /* Return number of bytes read */
  return no_bytes_read;
}

ssize_t dev_file_write( struct file *filp, char *user_buff,
                      size_t count, loff_t *f_pos) {
  int bytes_written = 0;

	// if(count>buffer_size)
	// {
	// 	printk("In write, reallocating kernel buffer\n");
 //    kfree(kernel_buffer);
	// 	kernel_buffer = kmalloc(count, GFP_KERNEL);
	// 	if(kernel_buffer==NULL)
	// 	{
	// 		printk("In write, reallocation failed\n");
	// 		return -1;
	// 	}
	// 	else
	// 	{
	// 		buffer_size = count;
	// 	}
	// }

  /* Use copy_from_user() to transfer data from user space */
  bytes_written = copy_from_user(kernel_buffer, user_buff, count);

  printk("No of bytes written %d\n", bytes_written);
  bytes_written = count - bytes_written;
  kernel_buffer[bytes_written] = '\0';

  printk("fpos val %d and memory buffer strlen %d\n", kernel_buffer[bytes_written], strlen(kernel_buffer));

  /* Return number of bytes written */
  return bytes_written;

}
