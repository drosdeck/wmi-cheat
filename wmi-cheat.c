#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/acpi.h>
#include <linux/input.h>
#include <linux/input/sparse-keymap.h>
#include <linux/dmi.h>


MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Edson Juliano Drosdeck"); 
MODULE_DESCRIPTION("hack WMI Hotkey Driver"); 
MODULE_VERSION("0.1"); 


#define CHEAT_WMI_EVENT_GUID "ABBC0F72-8EA1-11D1-00A0-C90629100000"

static struct input_dev *cheat_wmi_input_dev;
  
 static const struct key_entry cheat_wmi_keymap[] = {
	 	{ KE_KEY, 0x1c, { KEY_BRIGHTNESSDOWN } },
		        { KE_END, 0},
			    };
//
static void cheat_wmi_notify(u32 value, void *context)
    {
	struct acpi_buffer response = { ACPI_ALLOCATE_BUFFER, NULL };
	union acpi_object *obj;
	acpi_status status;
	int code;

	status = wmi_get_event_data(value, &response);
	if (status != AE_OK) {
	    pr_err("bad event status 0x%x\n", status);
	    return;
	}

	obj = (union acpi_object *)response.pointer;
	if (obj && obj->type == ACPI_TYPE_INTEGER) {
		code = obj->integer.value;
               printk("Codigo %d\n",code);
               printk("Codigo %x\n",code);
   	    if (!sparse_keymap_report_event(cheat_wmi_input_dev, code, 1, true))
		pr_info("Unknown key %x pressed\n", code);
	}

	kfree(obj);
    }

static int cheat_wmi_input_setup(void)
{
      
	int err;
        acpi_status status;

	cheat_wmi_input_dev = input_allocate_device();
      if (!cheat_wmi_input_dev)
		return -ENOMEM;
      cheat_wmi_input_dev->name = "Hack laptop WMI hotkeys";
      cheat_wmi_input_dev->phys = "wmi/input0";
      cheat_wmi_input_dev->id.bustype = BUS_HOST;

     err = sparse_keymap_setup(cheat_wmi_input_dev,cheat_wmi_keymap, NULL);
      	if (err){
		pr_info("Unable to setup device keymap\n");
                goto err_free_dev;
	}
  
     err = input_register_device(cheat_wmi_input_dev);
        if (err){
	   	pr_info("Unable to register input device\n");
		goto err_free_dev;
	}	


	//
//     if (!sparse_keymap_report_event(cheat_wmi_input_dev, 28, 1, true))
//	     		pr_info("Unknown key %x pressed\n", 28);

//        status = wmi_install_notify_handler(CHEAT_WMI_EVENT_GUID,cheat_wmi_notify, NULL);
//		if (ACPI_FAILURE(status)) {
//		err = -EIO;
//		goto err_free_dev;
//		}




       return 0;
err_free_dev:
		input_free_device(cheat_wmi_input_dev);
			return err;
}

static int __init cheat_start(void) 
{
	acpi_status status;
        int err;

        if (!wmi_has_guid(CHEAT_WMI_EVENT_GUID)) {
	    printk("No known WMI GUID found\n");
	    return -ENODEV;
	}

 	err = cheat_wmi_input_setup();

        if (err)
		return err;


	status = wmi_install_notify_handler(CHEAT_WMI_EVENT_GUID,
					cheat_wmi_notify, NULL);
		if (ACPI_FAILURE(status)) {
	    	
		return -ENODEV;
		}
	return 0; 
} 

static void __exit cheat_end(void) 
{ 
	printk(KERN_INFO "Goodbye Mr.\n"); 
       wmi_remove_notify_handler(CHEAT_WMI_EVENT_GUID);
} 

module_init(cheat_start); 
module_exit(cheat_end); 

