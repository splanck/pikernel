# Project Status

## Completed
- Implemented UART initialization and output.
- Implemented GPIO initialization and control.
- Added basic delay function using ARM's generic timer.
- Successfully tested UART input and output, GPIO blinking, and delay.
- Implemented framebuffer initialization.
- Successfully tested basic framebuffer setup and pixel drawing.

## Next Steps
1. **Handling Interrupts**
   - Set up a basic interrupt service routine (ISR).
   - Configure the interrupt controller.

2. **Implementing a Simple Scheduler**
   - Create a basic task structure.
   - Implement context switching.

3. **Expanding Memory Management**
   - Set up basic memory allocation.
   - Implement simple paging or segmentation.

4. **Improving Framebuffer Usage**
   - Debug and ensure the framebuffer is correctly initialized.
   - Implement more complex drawing functions (e.g., lines, rectangles).

## Additional Notes
- Ensure the toolchain (`aarch64-linux-gnu-gcc`) is installed.
- Current build process uses the Makefile provided in the project directory.
- UART, GPIO, delay, and basic framebuffer functionality are confirmed to be working as expected.
- Current focus is on ensuring the framebuffer initialization and pixel drawing are stable and correctly bounded.

## Project Log

### 2024-05-30
- **Implemented**: UART, GPIO, and delay functionality.
- **Tested**: UART input and output, GPIO blinking, delay.

### 2024-06-02
- **Implemented**: Framebuffer initialization.
- **Debugged**: Basic framebuffer setup and pixel drawing.
- **Encountered Issues**: Segmentation faults during pixel drawing.

### 2024-06-03
- **Improved**: Bounds checking in the `draw_pixel` function.
- **Debugged**: Framebuffer initialization and pixel drawing.
- **Encountered Issues**: Drawing loop segmentation fault.

## Updated README

Hi, I previously worked on a kernel project where I implemented UART, GPIO, delay, and basic framebuffer functionality. I have documented my progress in `README.md` within the project directory. I would like to continue by working on handling interrupts. Could you help me set up a basic interrupt service routine (ISR) and configure the interrupt controller?
