#pragma once

// KeyBoard Class
class VirtualKeyboard {
    public:
        void Render();
		void SetFont(uint8_t* font);
		void Highlight(uint8_t x, uint8_t y, uint32_t highlightColor = color(255,255,0), bool hide = false);
		void ToggleShift();
        bool caps = false;
		int16_t x = 8;
		int16_t y = 482;
		int16_t xmargin = 10;
		int16_t ymargin = 10;
		uint8_t* font;
		int8_t xcursor = 0;
		int8_t ycursor = 0;
		int8_t yfix = 0;
		const char lower[27] = "abcdefghijklmnopqrstuvwxyz";
		const char characters[67] = "1234567890=!@#$%^&*_+-ABCDEFGHIJKLM\\|;:',./?NOPQRSTUVWXYZ[]{}()\"~`";
		const char specials[23] = "[SPACE] [BACK] [ENTER]";
		char activeKey[2] = "";
		int32_t keyColor = color(255, 255, 255);
};

void VirtualKeyboard::ToggleShift() {
	// call render to update the keyboard
	// will need something to only update the qwerty keys
	
	// set the color to black so that we cannot see the text
	this->keyColor = color(0, 0, 0);
	this->Render();
	this->caps = !this->caps;
	// set back to white
	this->keyColor = color(255, 255, 255);
	this->Render();
	// render highlighted key
	this->Highlight(this->xcursor, this->ycursor);

}

// x is between 0 and 21 (inclusive)
// y is between 0 and 2 (inclusive)
void VirtualKeyboard::Highlight(uint8_t xOffset = 0, uint8_t yOffset = 0, uint32_t highlightColor, bool hide) {
	// KEYBOARD
	// 1 2 3 4 5 6 7 8 9 0 = ! @ # $ % ^ & * _ + -  
	// A B C D E F G H I J K L M \ | ; : ' , . / ?   
	// N O P Q R S T U V W X Y Z [ ] { } ( ) " ~ `
	// [ S P A C E ]   [ B A C K ]   [ E N T E R ]

	// get active key
	// check if within first 13 chars on y=1,2
	if (xOffset < 13 && !this->caps && yOffset != 0 && yOffset != 3) {
		if (yOffset == 1) {
			this->activeKey[0] = this->lower[xOffset];
		} else {
			this->activeKey[0] = this->lower[xOffset + 13];
		}
	} else {
		this->activeKey[0] = this->characters[xOffset + (yOffset * 22)];
	}

	if (yOffset != 3) {
		DRAW_FONT(this->font, this->activeKey, this->x + this->xmargin * xOffset, this->y + this->ymargin * yOffset, highlightColor, 0);

		// draw rectangle around the character

		if (hide) {
			highlightColor = color(0, 0, 0);
		}
		line(this->x + this->xmargin * xOffset - 2, this->y + this->ymargin * yOffset - 2, this->x + this->xmargin * (xOffset + 1) - 1, this->y + this->ymargin * yOffset - 2, highlightColor);
		line(this->x + this->xmargin * xOffset - 2, this->y + this->ymargin * yOffset - 2, this->x + this->xmargin * xOffset - 2, this->y + this->ymargin * (yOffset + 1) - 1, highlightColor);
		line(this->x + this->xmargin * (xOffset + 1) - 1, this->y + this->ymargin * yOffset - 2, this->x + this->xmargin * (xOffset + 1) - 1, this->y + this->ymargin * (yOffset + 1) - 1, highlightColor);
		line(this->x + this->xmargin * xOffset - 2, this->y + this->ymargin * (yOffset + 1) - 1, this->x + this->xmargin * (xOffset + 1) - 1, this->y + this->ymargin * (yOffset + 1) - 1, highlightColor);
	} else {
	// else specials
		char specialChar[2] = "";

		if (xOffset == 0) {
			for (int i = 0; i < 7; i++) {
				specialChar[0] = this->specials[i];
				DRAW_FONT(this->font, specialChar, this->x + this->xmargin * (xOffset + i), this->y + this->ymargin * yOffset, highlightColor, 0);
			}
		} else if (xOffset == 1) {
			for (int i = 8; i < 14; i++) {
				specialChar[0] = this->specials[i];
				DRAW_FONT(this->font, specialChar, this->x + this->xmargin * (xOffset + i - 1), this->y + this->ymargin * yOffset, highlightColor, 0);
			}
		} else if (xOffset == 2) {
			for (int i = 15; i < 22; i++) {
				specialChar[0] = this->specials[i];
				DRAW_FONT(this->font, specialChar, this->x + this->xmargin * (xOffset + i - 2), this->y + this->ymargin * yOffset, highlightColor, 0);
			}
		}
	}
}

void VirtualKeyboard::SetFont(uint8_t* font) {
	this->font = font;
}

void VirtualKeyboard::Render() {

	// render normals
	for (int i = 0; i < 66; i++) {
		this->activeKey[0] = this->characters[i];
		// if in top row
		if (i < 22) {
			DRAW_FONT(this->font, this->activeKey, this->x + this->xmargin * i, this->y, this->keyColor, 0);
		// if in 2nd row
		} else if (i < 44) {
			// check for left 13 keys
			if (i < 35 && !this->caps) {
				this->activeKey[0] = this->lower[i - 22];
			}
			DRAW_FONT(this->font, this->activeKey, this->x + this->xmargin * (i - 22), this->y + this->ymargin, this->keyColor, 0);
		// 3rd
		} else {
			// check for left 13 keys
			if (i < 57 && !this->caps) {
				this->activeKey[0] = this->lower[i - 44 + 13];
			}
			DRAW_FONT(this->font, this->activeKey, this->x + this->xmargin * (i - 44), this->y + this->ymargin * 2, this->keyColor, 0);
		}
	}
	// render specials
	for (int i = 0; i < 23; i++) {
		this->activeKey[0] = this->specials[i];
		DRAW_FONT(this->font, this->activeKey, this->x + this->xmargin * i, this->y + this->ymargin * 3, this->keyColor, 0);
	}

}	
