volatile int pixel_buffer_start;
volatile char *character_buffer = (char *) 0xC9000000;

void plot_pixel(int x, int y, short int line_color) {
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void clear_screen() {
    for (int x = 0; x < 320; x++) {
        for (int y = 0; y < 240; y++) {
            plot_pixel(x, y, 0x0000);
        }
    }
}

void plot_string(int x, int y, char *text_ptr) {	
    int offset = (y << 7) + x;

    while (*(text_ptr)) {
        *(character_buffer + offset) = *(text_ptr);
        text_ptr++;
        offset++;
    }
}

void VGA_text_clean() {
    for(int y = 0; y < 60; y++){
        for(int x = 0; x < 80; x++){
            int offset = (y << 7) + x;
            *(character_buffer + offset) = 0;
            offset++;
        }
    }
}

int main(void) {
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    pixel_buffer_start = *pixel_ctrl_ptr;
	
    clear_screen();
	VGA_text_clean();
	
	char myStringIDK[4];
	myStringIDK[3] = '\0';
	myStringIDK[2] = 'y';
	myStringIDK[1] = 'e';
	myStringIDK[0] = 'h';
	plot_string(10, 10, myStringIDK);
	
    return 0;
}
