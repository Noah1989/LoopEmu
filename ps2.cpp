#include "ps2.h"

ps2Code ps2CodeFromEvent(QKeyEvent *ev) {
    ps2Code c{0,{}};

    if(ev->modifiers() & Qt::KeypadModifier) {
        switch(ev->key()) {
        case Qt::Key_Slash:
        case Qt::Key_Return:
            c.bytes[c.nbytes++] = 0xE0;
            break;
        }
    } else {
        switch(ev->key()) {
        case Qt::Key_Super_L:
        case Qt::Key_AltGr:
        case Qt::Key_Super_R:
        case Qt::Key_Menu:
        // TODO: right control case Qt::Key_Ctrl:
        case Qt::Key_Insert:
        case Qt::Key_Home:
        case Qt::Key_PageUp:
        case Qt::Key_Delete:
        case Qt::Key_End:
        case Qt::Key_PageDown:
        case Qt::Key_Up:
        case Qt::Key_Left:
        case Qt::Key_Down:
        case Qt::Key_Right:
            c.bytes[c.nbytes++] = 0xE0;
            break;
        }
    }

    if (ev->type() == QEvent::KeyRelease) {
        c.bytes[c.nbytes++] = 0xF0;
    }

    if(ev->modifiers() & Qt::KeypadModifier) {
        switch(ev->key()) {
        case Qt::Key_NumLock: c.bytes[c.nbytes++] = 0x77; break;
        case Qt::Key_Slash: c.bytes[c.nbytes++] = 0x4A; break;
        case Qt::Key_Asterisk: c.bytes[c.nbytes++] = 0x7C; break;
        case Qt::Key_Minus: c.bytes[c.nbytes++] = 0x7B; break;
        case Qt::Key_7: c.bytes[c.nbytes++] = 0x6C; break;
        case Qt::Key_8: c.bytes[c.nbytes++] = 0x75; break;
        case Qt::Key_9: c.bytes[c.nbytes++] = 0x7D; break;
        case Qt::Key_Plus: c.bytes[c.nbytes++] = 0x79; break;
        case Qt::Key_4: c.bytes[c.nbytes++] = 0x6B; break;
        case Qt::Key_5: c.bytes[c.nbytes++] = 0x73; break;
        case Qt::Key_6: c.bytes[c.nbytes++] = 0x74; break;
        case Qt::Key_1: c.bytes[c.nbytes++] = 0x69; break;
        case Qt::Key_2: c.bytes[c.nbytes++] = 0x72; break;
        case Qt::Key_3: c.bytes[c.nbytes++] = 0x7A; break;
        case Qt::Key_0: c.bytes[c.nbytes++] = 0x70; break;
        case Qt::Key_Period: c.bytes[c.nbytes++] = 0x71; break;
        case Qt::Key_Enter: c.bytes[c.nbytes++] = 0x5A; break;
        }
    } else {
        switch(ev->key()) {
        case Qt::Key_Escape: c.bytes[c.nbytes++] = 0x76; break;
        case Qt::Key_F1: c.bytes[c.nbytes++] = 0x05; break;
        case Qt::Key_F2: c.bytes[c.nbytes++] = 0x06; break;
        case Qt::Key_F3: c.bytes[c.nbytes++] = 0x04; break;
        case Qt::Key_F4: c.bytes[c.nbytes++] = 0x0C; break;
        case Qt::Key_F5: c.bytes[c.nbytes++] = 0x03; break;
        case Qt::Key_F6: c.bytes[c.nbytes++] = 0x0B; break;
        case Qt::Key_F7: c.bytes[c.nbytes++] = 0x83; break;
        case Qt::Key_F8: c.bytes[c.nbytes++] = 0x0A; break;
        case Qt::Key_F9: c.bytes[c.nbytes++] = 0x01; break;
        case Qt::Key_F10: c.bytes[c.nbytes++] = 0x09; break;
        case Qt::Key_F11: c.bytes[c.nbytes++] = 0x78; break;
        case Qt::Key_F12: c.bytes[c.nbytes++] = 0x07; break;
        case Qt::Key_Print: /* TODO */ break;
        case Qt::Key_ScrollLock: c.bytes[c.nbytes++] = 0x7E; break;
        case Qt::Key_Pause: /* TODO */ break;
        case Qt::Key_QuoteLeft:
        case Qt::Key_AsciiTilde: c.bytes[c.nbytes++] = 0x0E; break;
        case Qt::Key_1:
        case Qt::Key_Exclam: c.bytes[c.nbytes++] = 0x16; break;
        case Qt::Key_2:
        case Qt::Key_At: c.bytes[c.nbytes++] = 0x1E; break;
        case Qt::Key_3:
        case Qt::Key_NumberSign: c.bytes[c.nbytes++] = 0x26; break;
        case Qt::Key_4:
        case Qt::Key_Dollar: c.bytes[c.nbytes++] = 0x25; break;
        case Qt::Key_5:
        case Qt::Key_Percent: c.bytes[c.nbytes++] = 0x2E; break;
        case Qt::Key_6:
        case Qt::Key_AsciiCircum: c.bytes[c.nbytes++] = 0x36; break;
        case Qt::Key_7:
        case Qt::Key_Ampersand: c.bytes[c.nbytes++] = 0x3D; break;
        case Qt::Key_8:
        case Qt::Key_Asterisk: c.bytes[c.nbytes++] = 0x3E; break;
        case Qt::Key_9:
        case Qt::Key_ParenLeft: c.bytes[c.nbytes++] = 0x46; break;
        case Qt::Key_0:
        case Qt::Key_ParenRight: c.bytes[c.nbytes++] = 0x45; break;
        case Qt::Key_Minus:
        case Qt::Key_Underscore: c.bytes[c.nbytes++] = 0x4E; break;
        case Qt::Key_Equal:
        case Qt::Key_Plus: c.bytes[c.nbytes++] = 0x55; break;
        case Qt::Key_Backspace: c.bytes[c.nbytes++] = 0x66; break;
        case Qt::Key_Tab: c.bytes[c.nbytes++] = 0x0D; break;
        case Qt::Key_Q: c.bytes[c.nbytes++] = 0x15; break;
        case Qt::Key_W: c.bytes[c.nbytes++] = 0x1D; break;
        case Qt::Key_E: c.bytes[c.nbytes++] = 0x24; break;
        case Qt::Key_R: c.bytes[c.nbytes++] = 0x2D; break;
        case Qt::Key_T: c.bytes[c.nbytes++] = 0x2C; break;
        case Qt::Key_Y: c.bytes[c.nbytes++] = 0x35; break;
        case Qt::Key_U: c.bytes[c.nbytes++] = 0x3C; break;
        case Qt::Key_I: c.bytes[c.nbytes++] = 0x43; break;
        case Qt::Key_O: c.bytes[c.nbytes++] = 0x44; break;
        case Qt::Key_P: c.bytes[c.nbytes++] = 0x4D; break;
        case Qt::Key_BracketLeft:
        case Qt::Key_BraceLeft: c.bytes[c.nbytes++] = 0x54; break;
        case Qt::Key_BracketRight:
        case Qt::Key_BraceRight: c.bytes[c.nbytes++] = 0x5B; break;
        case Qt::Key_Backslash:
        case Qt::Key_Bar: c.bytes[c.nbytes++] = 0x5D; break;
        case Qt::Key_CapsLock: c.bytes[c.nbytes++] = 0x58; break;
        case Qt::Key_A: c.bytes[c.nbytes++] = 0x1C; break;
        case Qt::Key_S: c.bytes[c.nbytes++] = 0x1B; break;
        case Qt::Key_D: c.bytes[c.nbytes++] = 0x23; break;
        case Qt::Key_F: c.bytes[c.nbytes++] = 0x2B; break;
        case Qt::Key_G: c.bytes[c.nbytes++] = 0x34; break;
        case Qt::Key_H: c.bytes[c.nbytes++] = 0x33; break;
        case Qt::Key_J: c.bytes[c.nbytes++] = 0x3B; break;
        case Qt::Key_K: c.bytes[c.nbytes++] = 0x42; break;
        case Qt::Key_L: c.bytes[c.nbytes++] = 0x4B; break;
        case Qt::Key_Semicolon:
        case Qt::Key_Colon: c.bytes[c.nbytes++] = 0x4C; break;
        case Qt::Key_Apostrophe:
        case Qt::Key_QuoteDbl: c.bytes[c.nbytes++] = 0x52; break;
        case Qt::Key_Return: c.bytes[c.nbytes++] = 0x5A; break;
        case Qt::Key_Shift: c.bytes[c.nbytes++] = 0x12; break;
        case Qt::Key_Z: c.bytes[c.nbytes++] = 0x1A; break;
        case Qt::Key_X: c.bytes[c.nbytes++] = 0x22; break;
        case Qt::Key_C: c.bytes[c.nbytes++] = 0x21; break;
        case Qt::Key_V: c.bytes[c.nbytes++] = 0x2A; break;
        case Qt::Key_B: c.bytes[c.nbytes++] = 0x32; break;
        case Qt::Key_N: c.bytes[c.nbytes++] = 0x31; break;
        case Qt::Key_M: c.bytes[c.nbytes++] = 0x3A; break;
        case Qt::Key_Comma:
        case Qt::Key_Less: c.bytes[c.nbytes++] = 0x41; break;
        case Qt::Key_Period:
        case Qt::Key_Greater: c.bytes[c.nbytes++] = 0x49; break;
        case Qt::Key_Slash:
        case Qt::Key_Question: c.bytes[c.nbytes++] = 0x4A; break;
            // TODO: right shift case Qt::Key_Shift: c.bytes[c.nbytes++] = 0x59; break;
        case Qt::Key_Control: c.bytes[c.nbytes++] = 0x14; break;
        case Qt::Key_Meta: c.bytes[c.nbytes++] = 0x1F; break;
        case Qt::Key_Alt: c.bytes[c.nbytes++] = 0x11; break;
        case Qt::Key_Space: c.bytes[c.nbytes++] = 0x29; break;
        case Qt::Key_AltGr: c.bytes[c.nbytes++] = 0x11; break;
            // TODO: right meta case Qt::Key_Meta: c.bytes[c.nbytes++] = 0x27; break;
        case Qt::Key_Menu: c.bytes[c.nbytes++] = 0x2F; break;
            // TODO: right control case Qt::Key_Ctrl: c.bytes[c.nbytes++] = 0x14; break;
        case Qt::Key_Insert: c.bytes[c.nbytes++] = 0x70; break;
        case Qt::Key_Home: c.bytes[c.nbytes++] = 0x6C; break;
        case Qt::Key_PageUp: c.bytes[c.nbytes++] = 0x7D; break;
        case Qt::Key_Delete: c.bytes[c.nbytes++] = 0x71; break;
        case Qt::Key_End: c.bytes[c.nbytes++] = 0x69; break;
        case Qt::Key_PageDown: c.bytes[c.nbytes++] = 0x7A; break;
        case Qt::Key_Up: c.bytes[c.nbytes++] = 0x75; break;
        case Qt::Key_Left: c.bytes[c.nbytes++] = 0x6B; break;
        case Qt::Key_Down: c.bytes[c.nbytes++] = 0x72; break;
        case Qt::Key_Right: c.bytes[c.nbytes++] = 0x74; break;
        }
    }
    return c;
}
