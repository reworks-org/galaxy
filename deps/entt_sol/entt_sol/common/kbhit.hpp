#pragma once

#if WIN32
#include <conio.h>
#elif __linux
// https://www.flipcode.com/archives/_kbhit_for_Linux.shtml
#include <stdio.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>

int _kbhit() {
  constexpr auto STDIN = 0;
  if (static bool initialized{false}; !initialized) [[unlikely]] {
    // Use termios to turn off line buffering
    termios term;
    tcgetattr(STDIN, &term);
    term.c_lflag &= ~ICANON;
    tcsetattr(STDIN, TCSANOW, &term);
    setbuf(stdin, NULL);

    initialized = true;
  }

  int bytesWaiting;
  ioctl(STDIN, FIONREAD, &bytesWaiting);
  return bytesWaiting;
}
#endif
