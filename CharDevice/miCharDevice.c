#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> 

static int device_open (struct inode *, struct file *) ;
static int device_release (struct inode *, struct file *) ;
static ssize_t device_read (struct file *, char *, size_t , loff_t *) ;
static ssize_t device_write (struct file *, const char *, size_t , loff_t *);

#define DeviceName "CharDevice-TP0"
#define BufferSize 80

static int Major;
static int DeviceOpen = 0;

static char msj[BufferSize];
static char *msg_Ptr;

static struct file_operations fops = {
 .read = device_read,
 .write = device_write,
 .open = device_open,
 .release = device_release
};

int init_module(void){ 

    Major = register_chrdev(0, DeviceName, &fops);
    if (Major < 0) {
        printk(KERN_ALERT "No se pudo registrar el dispositivo con %d\n", Major);
        return Major;
    }

    printk(KERN_INFO "CharDevice: Driver %s registrado \n", DeviceName);
    printk(KERN_INFO "CharDevice: Se le a asigno el número %d.\n", Major);
    printk(KERN_INFO "CharDevice: Para comunicarse con el driver, cree un archivo dev con: 'sudo mknod /dev/%s c %d 0'.\n", DeviceName, Major);
    return 0;
}


void cleanup_module(void){
    /* Destructor */
    unregister_chrdev(Major, DeviceName);
    printk(KERN_INFO "CharDevice: Driver desregistrado \n") ;
}

static int device_open(struct inode *inode, struct file *file){
   
    if (DeviceOpen){
        return -EBUSY;
    }

    DeviceOpen++;
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode *inode, struct file *file){
    DeviceOpen--; 
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t * offset){
    
    int bytes_read = 0;
    if (*msg_Ptr == 0){
        return 0;
    }

    while (length && *msg_Ptr) {
        put_user(*(msg_Ptr++), buffer++);
        length--;
        bytes_read++;
    }

    return bytes_read;

}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off){
   
   int i;
   int key = 3;
   #ifdef DEBUG
        printk(KERN_INFO "device_write(%p,%s,%d)",filp,buff,len);
   #endif 
    for(i=0;i<len && i<BufferSize;i++){
        get_user(msj[i], buff+i);
        if(msj[i] >= 'a' && msj[i] <= 'z'){
            
            msj[i] = msj[i] + key;
            
            if(msj[i] > 'z'){
				msj[i] = msj[i] - 'z' + 'a' - 1;
			}

        } else if(msj[i] >= 'A' && msj[i]<= 'Z') {
            msj[i] = msj[i] + key;
			
			if(msj[i] > 'Z'){
				msj[i] = msj[i] - 'Z' + 'A' - 1;
			}
			
        }

    }

    msg_Ptr = msj;
    return i;
}

MODULE_LICENSE ("GPL") ;
MODULE_AUTHOR ("Urtubey Carlos") ;
MODULE_DESCRIPTION ("CharDevice TP0 SOR2") ;