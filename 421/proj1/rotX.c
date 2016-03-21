/** @Author Ted Adams (tadams2@umbc.edu)
 * this program creates two kernel space misc character devices that make a caesar cipher
 * -rotX    which houses the encryption key
 * -rotXctl which will encrypt the message
 */

/*
 * This file uses kernel-doc style comments, which is similar to
 * Javadoc and Doxygen-style comments.  See
 * ~/linux/Documentation/kernel-doc-nano-HOWTO.txt for details.
 */

/*
 * Getting compilation warnings?  The Linux kernel is written against
 * C89, which means:
 *  - No // comments, and
 *  - All variables must be declared at the top of functions.
 * Read ~/linux/Documentation/CodingStyle to ensure your project
 * compiles without warnings.
 */

#include <linux/init.h>
#include <linux/module.h>

#define PREFIX "ROT-X: "
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <linux/spinlock.h>


static char *rotX_buffer;
static struct miscdevice rotX;
static struct miscdevice rotXctl;
static const struct file_operations rotXctlFops;
static const struct file_operations rotXFops;
unsigned int encryption_key;
static DEFINE_SPINLOCK(lock);
unsigned long flags;
/**
 * rotX_read() - callback invoked when a process reads from /dev/rotX
 * @filp: process's file object that is reading from this device (ignored)
 * @ubuf: destination buffer to store key
 * @count: number of bytes in @ubuf
 * @ppos: file offset (in/out parameter)
 *
 * Write to @ubuf the ASCII string representation of the key (via
 * snprintf()), incrementing @ppos by the number of bytes written. If
 * @ppos already points to the end of the string, then this user has
 * reached the end of file.
 *
 * Return: number of bytes written to @ubuf, 0 on end of file, or
 * negative on error
 */
static ssize_t rotX_read(struct file *filp, char __user *ubuf,
			  size_t count, loff_t *ppos)
{
  int retval;
  spin_lock_irqsave(&lock, flags);
  
  count = min(count, sizeof(rotX_buffer));
  /* converting the unsigned integer key value to a printable value, returns -1 on failure*/
  if(snprintf(rotX_buffer, sizeof(encryption_key) + 1, "%d", encryption_key) > PAGE_SIZE)
    {
      spin_unlock_irqrestore(&lock, flags);
      return -1;
    }
  /*giving the user the key value, returns a negative value on failure*/
  retval = copy_to_user(ubuf, rotX_buffer, count);
  if(retval < 0)
    {
      spin_unlock_irqrestore(&lock, flags);
      return retval;
    }
  /* incrementing the file counter, returns the number of bytes written upon success*/
  if(*ppos == 0)
    {
      *ppos += count;
      spin_unlock_irqrestore(&lock, flags);
      return count;
    }
  /*end of file case returns 0*/
  else
    {
      spin_unlock_irqrestore(&lock, flags);
      return 0;
    }
  
}

/**
 * rotX_write() - callback invoked when a process writes to /dev/rotX
 * @filp: process's file object that is writing to this device (ignored)
 * @ubuf: source buffer of bytes from user
 * @count: number of bytes in @ubuf
 * @ppos: file offset (ignored)
 *
 * Interpreting the string at @ubuf as an ASCII value, convert it to
 * an unsigned value via kstrtouint_from_user. Store the resulting
 * value as the encryption key.
 *
 * Return: @count, or negative on error
 */
static ssize_t rotX_write(struct file *filp, const char __user * ubuf,
			  size_t count, loff_t * ppos)
{
  int retval;
  spin_lock_irqsave(&lock, flags);
  count = min(count, sizeof(rotX_buffer));
  /* converts the written value to an unsigned integer value and stored in the variable encryption_key*/
  retval = kstrtouint_from_user(ubuf, count, 0, &encryption_key);
  /* returns and error if the kstrtouint_from_user() fails*/ 
  if(retval < 0)
    {
      spin_unlock_irqrestore(&lock, flags);
      return retval;
    }
  /* mods the key by 26 to ensure its in a valid range*/
  encryption_key = encryption_key % 26;
  spin_unlock_irqrestore(&lock, flags);
  return count;
}

/**
 * rotX_mmap() - callback invoked when a process mmap()s to /dev/rotX
 * @filp: process's file object that is writing to this device (ignored)
 * @vma: virtual memory allocation object containing mmap() request
 *
 * Create a mapping from kernel memory (specifically, rotX_buffer)
 * into user space. Code based upon
 * http://bloggar.combitech.se/ldc/2015/01/21/mmap-memory-between-kernel-and-userspace/
 *
 * You do not need to do modify this function.
 *
 * Return: 0 on success, negative on error.
 */
static int rotX_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);
	unsigned long page = vmalloc_to_pfn(rotX_buffer);
	if (size > PAGE_SIZE)
		return -EIO;
	vma->vm_pgoff = 0;
	vma->vm_page_prot = PAGE_SHARED;
	if (remap_pfn_range(vma, vma->vm_start, page, size, vma->vm_page_prot))
		return -EAGAIN;
	return 0;
}

/**
 * rotXctl_write() - callback invoked when a process writes to
 * /dev/rotXctl
 * @filp: process's file object that is writing to this device (ignored)
 * @ubuf: source buffer from user
 * @count: number of bytes in @ubuf
 * @ppos: file offset (ignored)
 *
 * If @count is at least 2 and @ubuf begins with the string "go", then
 * encrypt the data buffer with the current encryption key. Otherwise,
 * consume @ubuf and do nothing; this is not an error.
 *
 * Return: @count, or negative on error
 */
static ssize_t rotXctl_write(struct file *filp, const char __user * ubuf,
			     size_t count, loff_t * ppos)
{
  
  int i;
  spin_lock_irqsave(&lock, flags);
  count = min(count, sizeof(rotX_buffer));
  /* if we have at least 2 characters and ubuf begins with g and o*/
  if (count >= 2 && (ubuf[0] == 'g' && ubuf[1] == 'o'))
    {
      i = 0;
      /* ensuring that we do not overflow our buffer and stops when we reach the integer value 0*/
      while((i < PAGE_SIZE ) && rotX_buffer[i] != 0 )
	{
	  /*encrypt upper case character*/
	  if(rotX_buffer[i] <= 90 && rotX_buffer[i] >= 65)
	    {
	      /*if the key goes above the upper bound on a upper case character wrap around*/
	      if ((rotX_buffer[i] + encryption_key) > 90)
		{
		  rotX_buffer[i] += (encryption_key - 26);
		}
	      else
		{
		  rotX_buffer[i] += encryption_key;
		}
	    }
	  /*encrypt lower case character*/
	  else if(rotX_buffer[i] <= 122 && rotX_buffer[i] >= 97)
	    {
	      /*if the key goes above the upper bound on a lower case character wrap around*/
	      if ((rotX_buffer[i] + encryption_key) > 122)
		{
		  rotX_buffer[i] += (encryption_key - 26);
		  
		}
	      else
		{
		  rotX_buffer[i] += encryption_key;
		  
		}
	    }
	  i += 1;
	}
    }
  else
    {
      spin_unlock_irqrestore(&lock, flags);
      return count;
    }
  spin_unlock_irqrestore(&lock, flags);
  return count;
	
}
/**
 * rotX_init() - entry point into the ROT-X kernel module
 * Return: 0 on successful initialization, negative on error
 */
static int __init rotX_init(void)
{
  int retval;
  int retval2;

  /* setting up our two file_operations structs*/
  static const struct file_operations rotX_fops =
    {
      .read  = &rotX_read,
      .write = &rotX_write,
      .mmap  = &rotX_mmap
    };
  static const struct file_operations rotXctl_fops = 
    {
      .write = &rotXctl_write
    };  
  spin_lock_irqsave(&lock, flags);

  /* giving values to our declared miscdevice struct for rotX and rotXctl*/
  rotX.minor = MISC_DYNAMIC_MINOR;
  rotX.name = "rotX";
  rotX.fops = &rotX_fops;
  rotX.mode = 0666;
 
  rotXctl.minor = MISC_DYNAMIC_MINOR;
  rotXctl.name = "rotXctl";
  rotXctl.fops = &rotXctl_fops;
  rotXctl.mode = 0666;
    

  retval = misc_register(&rotX);
  retval2 = misc_register(&rotXctl);

  /* filling rotX_buffer with a PAGE_SIZE worth of 0's*/
  rotX_buffer = vmalloc(PAGE_SIZE);
  memset(rotX_buffer, 0, PAGE_SIZE);
  
  /* if either device fails to initilize we return an error*/
  if(retval < 0 || retval2 < 0)
    {
      printk("failed to initilize misc devices");
      spin_unlock_irqrestore(&lock, flags);
      return -1;
    }
  pr_info(PREFIX "Hello, world!\n");
        spin_unlock_irqrestore(&lock, flags);
	return 0;
}

/**
 * rotX_exit() - called by kernel to clean up resources
 */
static void __exit rotX_exit(void)
{
  /* deregistering the rotX and rotXctl and freeing the memory allocated to rotX_buffer*/
  spin_lock_irqsave(&lock, flags);
  misc_deregister(&rotX);
  misc_deregister(&rotXctl);
  vfree(&rotX_buffer);
	pr_info(PREFIX "Goodbye, world!\n");
        spin_unlock_irqrestore(&lock, flags);
}

module_init(rotX_init);
module_exit(rotX_exit);

MODULE_DESCRIPTION("CS421 ROT-X driver");
MODULE_LICENSE("GPL");
