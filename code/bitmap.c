#include "bitmap.h"

static bitmap_struct bitmaps;

Bitmap* loadBitmap(const char* filename) {
    // allocating necessary size
    Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

    // open filename in read binary mode
    FILE *filePtr;
    filePtr = fopen(filename, "rb");
    if (filePtr == NULL)
        return NULL;

    // read the bitmap file header
    BitmapFileHeader bitmapFileHeader;
    fread(&bitmapFileHeader, 2, 1, filePtr);

    // verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.type != 0x4D42) {
        fclose(filePtr);
        return NULL;
    }

    int rd;
    do {
        if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
            break;
        if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
            break;
        if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
            break;
    } while (0);

    if (rd = !1) {
        fprintf(stderr, "Error reading file\n");
        exit(-1);
    }

    // read the bitmap info header
    BitmapInfoHeader bitmapInfoHeader;
    fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

    // move file pointer to the begining of bitmap data
    fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

    // allocate enough memory for the bitmap image data
    unsigned char* bitmapImage = (unsigned char*) malloc(
            bitmapInfoHeader.imageSize);

    // verify memory allocation
    if (!bitmapImage) {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    // read in the bitmap image data
    fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);

    // make sure bitmap image data was read
    if (bitmapImage == NULL) {
        fclose(filePtr);
        return NULL;
    }

    // close file and return bitmap image data
    fclose(filePtr);

    bmp->bitmapData = bitmapImage;
    bmp->bitmapInfoHeader = bitmapInfoHeader;

    return bmp;
}

void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment) {
    if (bmp == NULL)
        return;

    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;

    if (alignment == ALIGN_CENTER)
        x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
        x -= width;

    if (x + width < 0 || x > getHres() || y + height < 0
            || y > getVres())
        return;

    int xCorrection = 0;
    if (x < 0) {
        xCorrection = -x;
        drawWidth -= xCorrection;
        x = 0;

        if (drawWidth > getHres())
            drawWidth = getHres();
    } else if (x + drawWidth >= getHres()) {
        drawWidth = getHres() - x;
    }

    char* bufferStartPos;
    char* imgStartPos;

    int i, j;
    for (i = 0; i < height; i++) {
    	int pos = y + height - 1 - i;
    	if (pos < 0 || pos >= getVres())
    		continue;
    	for (j = 0; j < width; j++) {
    		bufferStartPos = getAddr();
    		bufferStartPos += (x * 2) + (pos * getHres() * 2) + (j * 2);
    		imgStartPos = bmp->bitmapData + xCorrection * 2 + j * 2 + i * width * 2;

    		if (*imgStartPos == (int) -32)
    			if (*(imgStartPos + 1) == (int) 7)
    				continue;
    			else
    				memcpy(bufferStartPos, imgStartPos, 2);
    		else
    			memcpy(bufferStartPos, imgStartPos, 2);
    	}
    }
}

void drawBitmap_by_line(Bitmap* bmp, int x, int y, Alignment alignment) {
    if (bmp == NULL)
        return;

    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;

    if (alignment == ALIGN_CENTER)
        x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
        x -= width;

    if (x + width < 0 || x > getHres() || y + height < 0
            || y > getVres())
        return;

    int xCorrection = 0;
    if (x < 0) {
        xCorrection = -x;
        drawWidth -= xCorrection;
        x = 0;

        if (drawWidth > getHres())
            drawWidth = getHres();
    } else if (x + drawWidth >= getHres()) {
        drawWidth = getHres() - x;
    }

    char* bufferStartPos;
    char* imgStartPos;

    int i;
    for (i = 0; i < height; i++) {
    	int pos = y + height - 1 - i;

    	if (pos < 0 || pos >= getVres())
    		continue;

    	bufferStartPos = getAddr();
    	bufferStartPos += x * 2 + pos * getHres() * 2;

    	imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;

    	memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
    }
}

void drawBitmap_inBuff(Bitmap* bmp, int x, int y, Alignment alignment, unsigned char **double_buffer) {
    if (bmp == NULL)
        return;

    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;

    if (alignment == ALIGN_CENTER)
        x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
        x -= width;

    if (x + width < 0 || x > getHres() || y + height < 0
            || y > getVres())
        return;

    int xCorrection = 0;
    if (x < 0) {
        xCorrection = -x;
        drawWidth -= xCorrection;
        x = 0;

        if (drawWidth > getHres())
            drawWidth = getHres();
    } else if (x + drawWidth >= getHres()) {
        drawWidth = getHres() - x;
    }

    char* bufferStartPos;
    char* imgStartPos;

    int i, j;
    for (i = 0; i < height; i++) {
    	int pos = y + height - 1 - i;
    	if (pos < 0 || pos >= getVres())
    		continue;
    	for (j = 0; j < width; j++) {
    		bufferStartPos = *double_buffer;
    		bufferStartPos += (x * 2) + (pos * getHres() * 2) + (j * 2);
    		imgStartPos = bmp->bitmapData + xCorrection * 2 + j * 2 + i * width * 2;

    		if (*imgStartPos == (int) -32)
    			if (*(imgStartPos + 1) == (int) 7)
    				continue;
    			else
    				memcpy(bufferStartPos, imgStartPos, 2);
    		else
    			memcpy(bufferStartPos, imgStartPos, 2);
    	}
    }
}

void drawBitmap_by_line_inBuff(Bitmap* bmp, int x, int y, Alignment alignment, unsigned char **double_buffer) {
    if (bmp == NULL)
        return;

    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;

    if (alignment == ALIGN_CENTER)
        x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
        x -= width;

    if (x + width < 0 || x > getHres() || y + height < 0
            || y > getVres())
        return;

    int xCorrection = 0;
    if (x < 0) {
        xCorrection = -x;
        drawWidth -= xCorrection;
        x = 0;

        if (drawWidth > getHres())
            drawWidth = getHres();
    } else if (x + drawWidth >= getHres()) {
        drawWidth = getHres() - x;
    }

    char* bufferStartPos;
    char* imgStartPos;

    int i;
    for (i = 0; i < height; i++) {
    	int pos = y + height - 1 - i;

    	if (pos < 0 || pos >= getVres())
    		continue;

    	bufferStartPos = *double_buffer;
    	bufferStartPos += x * 2 + pos * getHres() * 2;

    	imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;

    	memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
    }
}

void readAllBitmaps_universe() {
	bitmaps.welcome = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/fundo.bmp");
	printf(".");
	bitmaps.spaceToContinue = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/continue.bmp");
	printf(".");
	bitmaps.arrow = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/arrow.bmp");
	printf(".");
	bitmaps.arrowCleaner = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/arrowCleaner.bmp");
	printf(".");
	bitmaps.grid = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/grid.bmp");
	printf(".");
	bitmaps.pecamarela = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/pecaamarela.bmp");
	printf(".");
	bitmaps.pecavermelha = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/pecavermelha.bmp");
	printf(".");
	bitmaps.gamearrow = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/gamearrow.bmp");
	printf(".");
	bitmaps.gamearrowCleaner = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/gamearrowCleaner.bmp");
	printf(".");
	bitmaps.pieceCleaner = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/pecacleaner.bmp");
	printf(".");
	bitmaps.blank = loadBitmap("/home/lcom1415-t6g10-proj/images/blank.bmp");
	printf(".");
	bitmaps.play = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/play.bmp");
	printf(".");
	bitmaps.options = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/options.bmp");
	printf(".");
	bitmaps.exit_button = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/exit.bmp");
	printf(".");
	bitmaps.credits = loadBitmap("/home/lcom1415-t6g10-proj/images/universe/credits.bmp");
	printf(".");
	bitmaps.progressBar = loadBitmap("/home/lcom1415-t6g10-proj/images/progressBar.bmp");
	printf(".");
	bitmaps.progressPart = loadBitmap("/home/lcom1415-t6g10-proj/images/progressBar_15sec.bmp");
	printf(".");
	bitmaps.zero = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/0.bmp");
	printf(".");
	bitmaps.um = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/1.bmp");
	printf(".");
	bitmaps.dois = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/2.bmp");
	printf(".");
	bitmaps.tres = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/3.bmp");
	printf(".");
	bitmaps.quatro = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/4.bmp");
	printf(".");
	bitmaps.cinco = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/5.bmp");
	printf(".");
	bitmaps.seis = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/6.bmp");
	printf(".");
	bitmaps.sete = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/7.bmp");
	printf(".");
	bitmaps.oito = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/8.bmp");
	printf(".");
	bitmaps.nove = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/9.bmp");
	printf(".");
	bitmaps.doispontos = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/doispontos.bmp");
	printf(".");
	bitmaps.slash = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/slash.bmp");
	printf(".");
	bitmaps.monday = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/Monday.bmp");
	printf(".");
	bitmaps.tuesday = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/Tuesday.bmp");
	printf(".");
	bitmaps.wednesday = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/Wednesday.bmp");
	printf(".");
	bitmaps.thursday = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/Thursday.bmp");
	printf(".");
	bitmaps.friday = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/Friday.bmp");
	printf(".");
	bitmaps.saturday = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/Saturday.bmp");
	printf(".");
	bitmaps.sunday = loadBitmap("/home/lcom1415-t6g10-proj/images/rtc/Sunday.bmp");
	printf(".");
	bitmaps.maintitle = loadBitmap("/home/lcom1415-t6g10-proj/images/maintitle.bmp");
	printf(".");
	bitmaps.credits_screen = loadBitmap("/home/lcom1415-t6g10-proj/images/credits.bmp");
	printf(".");
	bitmaps.player1 = loadBitmap("/home/lcom1415-t6g10-proj/images/player1.bmp");
	printf(".");
	bitmaps.player2 = loadBitmap("/home/lcom1415-t6g10-proj/images/player2.bmp");
	printf(".");
	bitmaps.secondTitle = loadBitmap("/home/lcom1415-t6g10-proj/images/secondtitle.bmp");
	printf(".");
}

void readAllBitmaps_pokemon() {
	bitmaps.welcome = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/fundo.bmp");
	bitmaps.spaceToContinue = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/continue.bmp");
	bitmaps.arrow = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/arrow.bmp");
	bitmaps.arrowCleaner = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/arrowCleaner.bmp");
	bitmaps.grid = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/grid.bmp");
	bitmaps.pecamarela = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/pecaamarela.bmp");
	bitmaps.pecavermelha = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/pecavermelha.bmp");
	bitmaps.gamearrow = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/gamearrow.bmp");
	bitmaps.gamearrowCleaner = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/gamearrowCleaner.bmp");
	bitmaps.pieceCleaner = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/pecacleaner.bmp");
	bitmaps.blank = loadBitmap("/home/lcom1415-t6g10-proj/images/blank.bmp");
	bitmaps.play = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/play.bmp");
	bitmaps.options = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/options.bmp");
	bitmaps.exit_button = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/exit.bmp");
	bitmaps.credits = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/credits.bmp");
	bitmaps.grid = loadBitmap("/home/lcom1415-t6g10-proj/images/pokemon/grid.bmp");
}

void draw_mainTitle() { drawBitmap(bitmaps.maintitle, 162, 50, ALIGN_LEFT); }

void draw_player1() { drawBitmap_by_line(bitmaps.player1, 750, 150, ALIGN_LEFT); }

void draw_player2() { drawBitmap_by_line(bitmaps.player2, 750, 150, ALIGN_LEFT); }

void draw_tie() {
	bitmaps.tie = loadBitmap("/home/lcom1415-t6g10-proj/images/draw.bmp");
	drawBitmap(bitmaps.tie, 30, 240, ALIGN_LEFT);
}

void draw_p1Wins() {
	bitmaps.p1Wins = loadBitmap("/home/lcom1415-t6g10-proj/images/p1wins.bmp");
	drawBitmap(bitmaps.p1Wins, 30, 240, ALIGN_LEFT);
}

void draw_p2Wins() {
	bitmaps.p2Wins = loadBitmap("/home/lcom1415-t6g10-proj/images/p2wins.bmp");
	drawBitmap(bitmaps.p2Wins, 30, 240, ALIGN_LEFT);
}

void draw_creditsScreen() { drawBitmap(bitmaps.credits_screen, 50, 50, ALIGN_LEFT); }

void draw_secondTitle() { drawBitmap(bitmaps.secondTitle, 50, 50, ALIGN_LEFT); }

void draw_welcome() { drawBitmap_by_line(bitmaps.welcome, 0, 0, ALIGN_LEFT); }

void draw_options() { drawBitmap_by_line(bitmaps.blank, 0, 0, ALIGN_LEFT); }

void draw_spaceToContinue() { drawBitmap(bitmaps.spaceToContinue, 0, 600, ALIGN_LEFT); }

void draw_arrow(int x, int y) { drawBitmap(bitmaps.arrow, x, y, ALIGN_LEFT); }

void draw_arrowCleaner(int x, int y) { drawBitmap_by_line(bitmaps.arrowCleaner, x, y, ALIGN_LEFT); }

void draw_progressBar() { drawBitmap_by_line(bitmaps.progressBar, 725, 225, ALIGN_LEFT); }

void draw_progressBar_15sec(int n_secs) { drawBitmap(bitmaps.progressPart, (975-16*n_secs), 225, ALIGN_LEFT); }

void draw_grid() { drawBitmap(bitmaps.grid, 50, 200, ALIGN_LEFT); }

void draw_grid_inBuff(unsigned char **double_buffer) { drawBitmap_inBuff(bitmaps.grid, 50, 200, ALIGN_LEFT, double_buffer); }

void draw_peca_amarela(int x, int y, unsigned char **double_buffer) { drawBitmap_by_line_inBuff(bitmaps.pecamarela, x, y, ALIGN_CENTER, double_buffer); }

void draw_peca_vermelha(int x, int y, unsigned char **double_buffer) { drawBitmap_by_line_inBuff(bitmaps.pecavermelha, x, y, ALIGN_CENTER, double_buffer); }

void draw_peca_cleaner(int x, int y, unsigned char **double_buffer) { drawBitmap_by_line_inBuff(bitmaps.pieceCleaner, x, y, ALIGN_CENTER, double_buffer); }

void draw_game_arrow(int col) { drawBitmap(bitmaps.gamearrow, (100 + (col - 1) * 90), 160, ALIGN_CENTER); }

void draw_game_arrow_cleaner(int col) { drawBitmap_by_line(bitmaps.gamearrowCleaner, (100 + (col - 1) * 90), 160, ALIGN_CENTER); }

void draw_menu_symbols() {
	drawBitmap(bitmaps.options, 100, 400, ALIGN_LEFT);
	drawBitmap(bitmaps.credits, 100, 500, ALIGN_LEFT);
	drawBitmap(bitmaps.exit_button, 100, 600, ALIGN_LEFT);
	drawBitmap(bitmaps.play, 100, 300, ALIGN_LEFT);
	return;
}

void low_red_piece(int empty_spaces, int col, unsigned char **double_buffer) {
	//-------timer-------
	game.clock_time = 0;
	int ipc_status, r;
	message msg;
	//------timer-f----

	float spY = 5.35;
	float y = 125;
	float yf = 90 * empty_spaces/6;

	while((game.clock_time < yf)){
		//---------------timer------------------------------
		r = driver_receive(ANY, &msg, &ipc_status);

		if ( r != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		memcpy(*double_buffer, getAddr(), 1024*768*2);
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & game.timer_irq) {
					if (game.clock_time == 0) {
						draw_peca_vermelha((100 + (col - 1) * 90), y, double_buffer);
						draw_grid_inBuff(double_buffer);
					}
					draw_peca_cleaner((100 + (col - 1) * 90), y, double_buffer);
					timer_int_handler();
					y += spY;
					draw_peca_vermelha((100 + (col - 1) * 90), y, double_buffer);
					draw_grid_inBuff(double_buffer);
					memcpy(getAddr(), *double_buffer, 1024*768*2);
				}
				break;
			default:
				break;
			}
		} else {}
		//---------------timer-f---------------------------
	}

	return;
}

void low_yellow_piece(int empty_spaces, int col, unsigned char **double_buffer) {
	//-------timer-------
		game.clock_time = 0;
		int ipc_status, r;
		message msg;
		//------timer-f----
		float spY = 5.35;
		float y = 125;
		float yf = 90 * empty_spaces/6;


		while((game.clock_time < yf)){
			//---------------timer------------------------------
			r = driver_receive(ANY, &msg, &ipc_status);

			if ( r != 0 ) {
				printf("driver_receive failed with: %d", r);
				continue;
			}

			memcpy(*double_buffer, getAddr(), 1024*768*2);

			if (is_ipc_notify(ipc_status)) {
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.NOTIFY_ARG & game.timer_irq) {
						if (clock_time == 0) {
							draw_peca_amarela((100 + (col - 1) * 90), y, double_buffer);
							draw_grid_inBuff(double_buffer);
						}
						draw_peca_cleaner((100 + (col - 1) * 90), y, double_buffer);
						timer_int_handler();
						y += spY;
						draw_peca_amarela((100 + (col - 1) * 90), y, double_buffer);
						draw_grid_inBuff(double_buffer);
						memcpy(getAddr(), *double_buffer, 1024*768*2);
					}
					break;
				default:
					break;
				}
			} else {}
			//---------------timer-f---------------------------
		}

		return;
}

void draw_options_arrow(int xArrowPos,int yArrowPos) {
	if (xArrowPos == 350) {
		if (yArrowPos == 450) {
			Bitmap* temp;
			temp = loadBitmap("/home/lcom1415-t6g10-proj/images/rocket.bmp");
			drawBitmap(temp, xArrowPos, yArrowPos, ALIGN_LEFT);
		}
		else {
			Bitmap* temp;
			temp = loadBitmap("/home/lcom1415-t6g10-proj/images/squirtle.bmp");
			drawBitmap(temp, xArrowPos, yArrowPos, ALIGN_LEFT);
		}
	}
	else {

	}
}

void draw_0(int x, int y) { drawBitmap(bitmaps.zero, x, y, ALIGN_LEFT); }

void draw_1(int x, int y) { drawBitmap(bitmaps.um, x, y, ALIGN_LEFT); }

void draw_2(int x, int y) { drawBitmap(bitmaps.dois, x, y, ALIGN_LEFT); }

void draw_3(int x, int y) { drawBitmap(bitmaps.tres, x, y, ALIGN_LEFT); }

void draw_4(int x, int y) { drawBitmap(bitmaps.quatro, x, y, ALIGN_LEFT); }

void draw_5(int x, int y) { drawBitmap(bitmaps.cinco, x, y, ALIGN_LEFT); }

void draw_6(int x, int y) { drawBitmap(bitmaps.seis, x, y, ALIGN_LEFT); }

void draw_7(int x, int y) { drawBitmap(bitmaps.sete, x, y, ALIGN_LEFT); }

void draw_8(int x, int y) { drawBitmap(bitmaps.oito, x, y, ALIGN_LEFT); }

void draw_9(int x, int y) { drawBitmap(bitmaps.nove, x, y, ALIGN_LEFT); }

void draw_doispontos() { drawBitmap(bitmaps.doispontos, 40, 30, ALIGN_LEFT); }

void draw_slash(int x) { drawBitmap(bitmaps.slash, x, 10, ALIGN_LEFT); }

void draw_monday() { drawBitmap(bitmaps.monday, 170, 10, ALIGN_LEFT); }

void draw_tuesday() { drawBitmap(bitmaps.tuesday, 170, 10, ALIGN_LEFT); }

void draw_wednesday() { drawBitmap(bitmaps.wednesday, 170, 10, ALIGN_LEFT); }

void draw_thursday() { drawBitmap(bitmaps.thursday, 170, 10, ALIGN_LEFT); }

void draw_friday() { drawBitmap(bitmaps.friday, 170, 10, ALIGN_LEFT); }

void draw_saturday() { drawBitmap(bitmaps.saturday, 170, 10, ALIGN_LEFT); }

void draw_sunday() { drawBitmap(bitmaps.sunday, 170, 10, ALIGN_LEFT); }

void draw_year(int year) {
	draw_2(90,10);
	draw_0(105,10);
	if (year < 10) {
		draw_0(120,10);
		if (year == 1)
			draw_1(135,10);
		else if (year == 2)
			draw_2(135,10);
		else if (year == 3)
			draw_3(135,10);
		else if (year == 4)
			draw_4(135,10);
		else if (year == 5)
			draw_5(135,10);
		else if (year == 6)
			draw_6(135,10);
		else if (year == 7)
			draw_7(135,10);
		else if (year == 8)
			draw_8(135,10);
		else if (year == 9)
			draw_9(135,10);
		else
			draw_0(135,10);
	}
	else {
		if ((year%10) == 1)
			draw_1(135,10);
		else if ((year % 10) == 2)
			draw_2(135,10);
		else if ((year % 10) == 3)
			draw_3(135,10);
		else if ((year % 10) == 4)
			draw_4(135,10);
		else if ((year % 10) == 5)
			draw_5(135,10);
		else if ((year % 10) == 6)
			draw_6(135,10);
		else if ((year % 10) == 7)
			draw_7(135,10);
		else if ((year % 10) == 8)
			draw_8(135,10);
		else if ((year % 10) == 9)
			draw_9(135,10);
		else
			draw_0(135,10);

		if ((year / 10)== 1)
			draw_1(120,10);
		else if ((year / 10)== 2)
			draw_2(120,10);
		else if ((year / 10)== 3)
			draw_3(120,10);
		else if ((year / 10)== 4)
			draw_4(120,10);
		else if ((year / 10)== 5)
			draw_5(120,10);
		else if ((year / 10)== 6)
			draw_6(120,10);
		else if ((year / 10)== 7)
			draw_7(120,10);
		else if ((year / 10)== 8)
			draw_8(120,10);
		else if ((year / 10)== 9)
			draw_9(120,10);
		else
			draw_0(120,10);
	}
}

void draw_day(int day) {
	if (day < 10) {
		draw_0(10,10);
		if (day == 1)
			draw_1(25,10);
		else if (day == 2)
			draw_2(25,10);
		else if (day == 3)
			draw_3(25,10);
		else if (day == 4)
			draw_4(25,10);
		else if (day == 5)
			draw_5(25,10);
		else if (day == 6)
			draw_6(25,10);
		else if (day == 7)
			draw_7(25,10);
		else if (day == 8)
			draw_8(25,10);
		else if (day == 9)
			draw_9(25,10);
		else
			draw_0(25,10);
	}
	else {
		if ((day%10) == 1)
			draw_1(25,10);
		else if ((day % 10) == 2)
			draw_2(25,10);
		else if ((day % 10) == 3)
			draw_3(25,10);
		else if ((day % 10) == 4)
			draw_4(25,10);
		else if ((day % 10) == 5)
			draw_5(25,10);
		else if ((day % 10) == 6)
			draw_6(25,10);
		else if ((day % 10) == 7)
			draw_7(25,10);
		else if ((day % 10) == 8)
			draw_8(25,10);
		else if ((day % 10) == 9)
			draw_9(25,10);
		else
			draw_0(25,10);

		if ((day / 10) == 1)
			draw_1(10,10);
		else if ((day / 10) == 2)
			draw_2(10,10);
		else if ((day / 10) == 3)
			draw_3(10,10);
		else if ((day / 10) == 4)
			draw_4(10,10);
		else if ((day / 10) == 5)
			draw_5(10,10);
		else if ((day / 10) == 6)
			draw_6(10,10);
		else if ((day / 10) == 7)
			draw_7(10,10);
		else if ((day / 10) == 8)
			draw_8(10,10);
		else if ((day / 10) == 9)
			draw_9(10,10);
		else
			draw_0(10,10);
	}
}

void draw_hour(int hour) {
	if (hour < 10) {
		draw_0(10,30);
		if (hour == 1)
			draw_1(25,30);
		else if (hour == 2)
			draw_2(25,30);
		else if (hour == 3)
			draw_3(25,30);
		else if (hour == 4)
			draw_4(25,30);
		else if (hour == 5)
			draw_5(25,30);
		else if (hour == 6)
			draw_6(25,30);
		else if (hour == 7)
			draw_7(25,30);
		else if (hour == 8)
			draw_8(25,30);
		else if (hour == 9)
			draw_9(25,30);
		else
			draw_0(25,30);
	}
	else {
		if ((hour%10) == 1)
			draw_1(25,30);
		else if ((hour % 10) == 2)
			draw_2(25,30);
		else if ((hour % 10) == 3)
			draw_3(25,30);
		else if ((hour % 10) == 4)
			draw_4(25,30);
		else if ((hour % 10) == 5)
			draw_5(25,30);
		else if ((hour % 10) == 6)
			draw_6(25,30);
		else if ((hour % 10) == 7)
			draw_7(25,30);
		else if ((hour % 10) == 8)
			draw_8(25,30);
		else if ((hour % 10) == 9)
			draw_9(25,30);
		else
			draw_0(25,30);

		if ((hour / 10) == 1)
			draw_1(10,30);
		else if ((hour / 10) == 2)
			draw_2(10,30);
		else if ((hour / 10) == 3)
			draw_3(10,30);
		else if ((hour / 10) == 4)
			draw_4(10,30);
		else if ((hour / 10) == 5)
			draw_5(10,30);
		else if ((hour / 10) == 6)
			draw_6(10,30);
		else if ((hour / 10) == 7)
			draw_7(10,30);
		else if ((hour / 10) == 8)
			draw_8(10,30);
		else if ((hour / 10) == 9)
			draw_9(10,30);
		else
			draw_0(10,30);
	}
}

void draw_minute(int minute) {
	if (minute < 10) {
		draw_0(55,30);
		if (minute == 1)
			draw_1(75,30);
		else if (minute == 2)
			draw_2(75,30);
		else if (minute == 3)
			draw_3(75,30);
		else if (minute == 4)
			draw_4(75,30);
		else if (minute == 5)
			draw_5(75,30);
		else if (minute == 6)
			draw_6(75,30);
		else if (minute == 7)
			draw_7(75,30);
		else if (minute == 8)
			draw_8(75,30);
		else if (minute == 9)
			draw_9(75,30);
		else
			draw_0(75,30);
	}
	else {
		if ((minute%10) == 1)
			draw_1(75,30);
		else if ((minute % 10) == 2)
			draw_2(75,30);
		else if ((minute % 10) == 3)
			draw_3(75,30);
		else if ((minute % 10) == 4)
			draw_4(75,30);
		else if ((minute % 10) == 5)
			draw_5(75,30);
		else if ((minute % 10) == 6)
			draw_6(75,30);
		else if ((minute % 10) == 7)
			draw_7(75,30);
		else if ((minute % 10) == 8)
			draw_8(75,30);
		else if ((minute % 10) == 9)
			draw_9(75,30);
		else
			draw_0(75,30);

		if ((minute / 10) == 1)
			draw_1(55,30);
		else if ((minute / 10) == 2)
			draw_2(55,30);
		else if ((minute / 10) == 3)
			draw_3(55,30);
		else if ((minute / 10) == 4)
			draw_4(55,30);
		else if ((minute / 10) == 5)
			draw_5(55,30);
		else if ((minute / 10) == 6)
			draw_6(55,30);
		else if ((minute / 10) == 7)
			draw_7(55,30);
		else if ((minute / 10) == 8)
			draw_8(55,30);
		else if ((minute / 10) == 9)
			draw_9(55,30);
		else
			draw_0(55,30);
	}
}

void draw_date() {
	rtc_get();

	//-------------draw-day------------
	int day1 = date.day >> 4;
	int day2 = date.day & 0x0F;
	int day3 = day1 * 10 + day2;
	draw_day(day3);


	draw_slash(40);
	//-------------draw-month------------
	if  (date.month < 10)
		draw_0(50,10);

	if (date.month == 1)
		draw_1(65,10);
	else if (date.month == 2)
		draw_2(65,10);
	else if (date.month == 3)
		draw_3(65,10);
	else if (date.month == 4)
		draw_4(65,10);
	else if (date.month == 5)
		draw_5(65,10);
	else if (date.month == 6)
		draw_6(65,10);
	else if (date.month == 7)
		draw_7(65,10);
	else if (date.month == 8)
		draw_8(65,10);
	else if (date.month == 9)
		draw_9(65,10);
	else if (date.month == 16) {
		draw_1(50,10);
		draw_0(65,10);
	}
	else if (date.month == 17) {
		draw_1(50,10);
		draw_1(65,10);
	}
	else {

		draw_1(50,10);
		draw_2(65,10);
}


	draw_slash(80);
	//-------------draw-year------------
	int year1 = date.year >> 4;
	int year2 = date.year & 0x0F;
	int year3 = year1 * 10 + year2;
	draw_year(year3);

	//-------------draw-weekday------------
	if (date.weekday == 1)
		draw_thursday();
	else if (date.weekday == 2)
		draw_friday();
	else if (date.weekday ==  3)
		draw_saturday();
	else if (date.weekday == 4)
		draw_sunday();
	else if (date.weekday == 5)
		draw_monday();
	else if (date.weekday == 6)
		draw_tuesday();
	else
		draw_wednesday();


	//-------------draw-hour------------
	int hour1 = date.hour >> 4;
	int hour2 = date.hour & 0x0F;
	int hour3 = hour1 * 10 + hour2;
	draw_hour(hour3);

	draw_doispontos();
	//-------------draw-minute------------
	int minute1 = date.minute >> 4;
	int minute2 = date.minute & 0x0F;
	int minute3 = minute1 * 10 + minute2;
	draw_minute(minute3);
}
