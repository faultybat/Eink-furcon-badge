
#include "EPD_5in65f.h"
#include "EPD_Test.h"
#define img_lim 16

//Defining the LDE pins
#define LED_PIN_1 28
#define LED_PIN_2 27
#define LED_PIN_3 26
#define LED_PIN_4 22

#define DELAY_AMMOUNT 500
//sleeping the display after 2 minuites (time cal : (1/D_M * 60 * TIME))
#define DISPLAY_SLEEP_TIME 240
//################### GLOBAL VARS #######################
//the counter for the curret selected image index (not the image shown)
int image_sellector = 0;
//the counter for the image that is shown
int selected_image = 0;
//the last selected image
int last_selected_image = 0;
//Loop count to get sleep
int  screen_sleep_loop_count = 0;
//loop count for the leds
int led_sleep_loop_count = 0;
//Screen sleep state
bool screen_in_sleep = false;
//Sleeping leds
bool sleep_leds = false;




void blank_screen(void){
    printf("Eink Badge V1\r\n");
	printf("e-Paper Init and Clear...\r\n");
	EPD_5IN65F_Init();
    printf("blanking screen.\r\n");
	EPD_5IN65F_Clear(EPD_5IN65F_WHITE);
	DEV_Delay_ms(100);
				
	UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
	UDOUBLE Imagesize = ((EPD_5IN65F_WIDTH % 2 == 0)? (EPD_5IN65F_WIDTH / 2 ): (EPD_5IN65F_WIDTH / 2 + 1)) * EPD_5IN65F_HEIGHT;
	Imagesize = Imagesize/4;
	printf("Not enough memory, only part of the window is displayed\r\n");
	printf("Imagesize %d\r\n",Imagesize);
	if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
			printf("Failed to apply for black memory...\r\n");
			return -1;
	}
	Paint_NewImage(BlackImage, EPD_5IN65F_WIDTH/2, EPD_5IN65F_HEIGHT/2, 0, EPD_5IN65F_WHITE);
	Paint_SetScale(7);
    EPD_5IN65F_Clear(EPD_5IN65F_WHITE);
    DEV_Delay_ms(1000);

}

void initalize(void){    
    //Initalizing the LEDS
    gpio_init(LED_PIN_1);
    gpio_init(LED_PIN_2);
    gpio_init(LED_PIN_3);
    gpio_init(LED_PIN_4);
    gpio_set_dir(LED_PIN_1, GPIO_OUT);
    gpio_set_dir(LED_PIN_2, GPIO_OUT);
    gpio_set_dir(LED_PIN_3, GPIO_OUT);
    gpio_set_dir(LED_PIN_4, GPIO_OUT);
    gpio_set_dir(KEY0, GPIO_IN);
    gpio_pull_up(KEY0);//Need to pull up
    gpio_set_dir(KEY1, GPIO_IN);
    gpio_pull_up(KEY1);//Need to pull up
    gpio_set_dir(KEY2, GPIO_IN);
    gpio_pull_up(KEY2);//Need to pull up
    gpio_put(LED_PIN_1, 1);
    gpio_put(LED_PIN_2, 1);
    gpio_put(LED_PIN_3, 1);
    gpio_put(LED_PIN_4, 1);
    //Initliasize the RTC
}



void select_image(void){
    switch (selected_image)
    {
    case 0:
    printf("Faulty profile 1");
    EPD_5IN65F_Display(gImage_5in65f_faulty);
        break;
    case 1:
    printf("Faulty ref sheat name");
    EPD_5IN65F_Display(gImage_5in65f_faulty_name);
        break;
    case 2:
    printf("Faulty ref sheat big");
    EPD_5IN65F_Display(gImage_5in65f_faulty_big);
        break;
    case 3:
    printf("Faulty ref sheat hi");
    EPD_5IN65F_Display(gImage_5in65f_faulty_hi);
        break;
    default:
        break;
    }
}


int myexmp(void){
    // ############### INIALSING
    printf("Starting Eink badge v1\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

	printf("e-Paper Init and Clear...\r\n");
	EPD_5IN65F_Init();
	EPD_5IN65F_Clear(EPD_5IN65F_WHITE);
	DEV_Delay_ms(100);
				
	UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough led_sleep_loop_countheap size */
	UDOUBLE Imagesize = ((EPD_5IN65F_WIDTH % 2 == 0)? (EPD_5IN65F_WIDTH / 2 ): (EPD_5IN65F_WIDTH / 2 + 1)) * EPD_5IN65F_HEIGHT;
	Imagesize = Imagesize/4;
	printf("Not enough memory, only part of the window is displayed\r\n");
	printf("Imagesize %d\r\n",Imagesize);
	if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
			printf("Failed to apply for black memory...\r\n");
			return -1;
	}
	Paint_NewImage(BlackImage, EPD_5IN65F_WIDTH/2, EPD_5IN65F_HEIGHT/2, 0, EPD_5IN65F_WHITE);
	Paint_SetScale(7);
    initalize();
    
    //################ MAIN CONTROL LOOP
    while (1)
    {
    //Button 0 pressed (increase image counter)
    if(DEV_Digital_Read(KEY0 ) == 0)
    {
        if (sleep_leds == true){
        sleep_leds = false;
        led_sleep_loop_count = 0;
        }
        else{
            printf("button 0 pressed \n");
            image_sellector = image_sellector + 1;
            led_sleep_loop_count = 0;
            if (image_sellector > img_lim){
                image_sellector = 0;
            }
        }
    }

    //button 1 pressed (decreese image counter)
    if(DEV_Digital_Read(KEY1 ) == 0)
    {
        if (sleep_leds == true){
            sleep_leds = false;
            led_sleep_loop_count = 0;
        }
        else{
            printf("button 1 pressed \n");
            image_sellector = image_sellector - 1;
            led_sleep_loop_count = 0;
            if (image_sellector < 0){
                image_sellector =  img_lim;
            }
        }
    }

    //button 2 pressed (select image)
    if(DEV_Digital_Read(KEY2 ) == 0)
    {
        if (sleep_leds == true){
            sleep_leds = false;
            led_sleep_loop_count = 0;
        }
        else{
            if (screen_in_sleep == true){
                printf("The display was in sleep mode, wakeing up !!!");
                EPD_5IN65F_Init();
                DEV_Delay_ms(100);
            }
            led_sleep_loop_count = 0;
            screen_sleep_loop_count = 0;
            printf("button 2 pressed \n");
            selected_image = image_sellector;
            select_image();
        }
    }

    if (sleep_leds == false)
    {
    if((image_sellector % 2) > 0) { gpio_put(LED_PIN_1, 1); } else { gpio_put(LED_PIN_1, 0); }
    if((image_sellector % 4) > 1) { gpio_put(LED_PIN_2, 1); } else { gpio_put(LED_PIN_2, 0); }
    if((image_sellector % 8) > 3) { gpio_put(LED_PIN_3, 1); } else { gpio_put(LED_PIN_3, 0); }
    if((image_sellector % 16) > 7) { gpio_put(LED_PIN_4, 1); } else { gpio_put(LED_PIN_4, 0); }
    } else {
    gpio_put(LED_PIN_1, 0);
    gpio_put(LED_PIN_2, 0);
    gpio_put(LED_PIN_3, 0);
    gpio_put(LED_PIN_4, 0);
    led_sleep_loop_count = 0;
    }

    if (sleep_leds == false){
    led_sleep_loop_count = led_sleep_loop_count + 1;
    printf("      : led sleep \r%i\n",led_sleep_loop_count);
    }

    if (screen_in_sleep == false){
    screen_sleep_loop_count = screen_sleep_loop_count + 1;
    printf("     : screen sleep \r%i\n",screen_sleep_loop_count);
    }

    //Top prevent the LEDS draining to much power turn them off
    if (led_sleep_loop_count >= 30 && sleep_leds == false){
    sleep_leds = true;
    led_sleep_loop_count = 0;
     printf("Turning off the leds");
    }
    //To prevent damage to the display the screen is sleeped after
    //the time set in the display sleep time. 
    if (screen_sleep_loop_count >= DISPLAY_SLEEP_TIME){
        printf("sleeping the display");
        screen_in_sleep = true;
        screen_sleep_loop_count = 0;
        EPD_5IN65F_Sleep();
    }
    
    DEV_Delay_ms(DELAY_AMMOUNT);
    }
}

