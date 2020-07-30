#include <atmel_start.h>
#include <stdio.h>
#include <timeout.h>

const uint32_t measure_cycles = 1000;
const uint32_t drive_cycles   = 1000;
const uint32_t display_cycles = 4000;
const uint32_t process_cycles = 4000;
const uint32_t clock1_cycles  = 8000;
const uint32_t clock2_cycles  = 16000;

static uint16_t clock_1 = 0;
static uint16_t clock_2 = 0;

static absolutetime_t measure();
static absolutetime_t drive();
static absolutetime_t display();
static absolutetime_t process();
static absolutetime_t clock1();
static absolutetime_t clock2();

volatile uint8_t      i;
volatile adc_result_t measurement;
volatile adc_result_t duty;

// Initialize the two first elements in the struct, the
// remaining elements are initialized by TIMER_0_timeout_create()
timer_struct_t measure_handle = {measure, NULL};
timer_struct_t drive_handle   = {drive, NULL};
timer_struct_t display_handle = {display, NULL};
timer_struct_t process_handle = {process, NULL};
timer_struct_t clock1_handle  = {clock1, NULL};
timer_struct_t clock2_handle  = {clock2, NULL};

static absolutetime_t measure()
{
	// Get conversion from ADC CH0
	measurement = ADC_0_get_conversion(0);

	// Schedule next measure() to be called
	return measure_cycles;
}

static absolutetime_t drive()
{
	// Get 8 MSB
	duty = measurement >> (ADC_0_get_resolution() - 8);

	// Output duty cycle on PWM CH0(PD6) as read from ADC
	PWM_0_load_duty_cycle_ch0(duty);

	// Schedule next drive() to be called
	return drive_cycles;
}

static absolutetime_t display()
{
	printf("ADC=%d\r\n", measurement);
	printf("DutyC=%d\r\n", duty);

	// Schedule next display() to be called
	return display_cycles;
}

static absolutetime_t process()
{
	measure();
	drive();
	display();

	// Schedule next display() to be called
	return process_cycles;
}

static absolutetime_t clock1()
{
	printf("clock1=%d\r\n", clock_1++);

	// Schedule next clock1() to be called
	return clock1_cycles;
}

static absolutetime_t clock2()
{
	printf("clock2=%d\r\n", clock_2++);

	// Schedule next clock2() to be called
	return clock2_cycles;
}

extern timer_struct_t *TIMER_0_list_head;

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	sei();

	// Add tasks to scheduler

	TIMER_0_timeout_create(&measure_handle, measure_cycles);
	TIMER_0_timeout_create(&drive_handle, drive_cycles);
	TIMER_0_timeout_create(&display_handle, display_cycles);

	// TIMER_0_timeout_create(&process_handle, process_cycles);

	TIMER_0_timeout_create(&clock1_handle, clock1_cycles);
	TIMER_0_timeout_create(&clock2_handle, clock2_cycles);

	while (1) {
		// Returns immediately if no callback is ready to execute
		TIMER_0_timeout_call_next_callback();
	}
}
