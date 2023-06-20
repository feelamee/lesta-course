# This is specification of tetris #

When game is run player see __main menu__ with:
- start button
- __difficulty level__ selector
- dashboard for score
- maybe quit button
- settings button
  - slider for volume
  - config path
  - fields for remapping control buttons
  - button for reset settings
  
size of window same as size of phone - always in vertical position,
if it resize or open fullscreen => background will fill all space
  
on button start:
  - all buttons disappear and __game map__ will appear
  - background music change.
  - near __game map__ will be __dashboard__ for list of next __tetromino__

__dashboard__ is rectangle with header and usefull space for info

__tetromino__ is blocks, which appear on the top of __game map__,
and each delta(depends on __difficulty level__) it will fall on one square,
if any part of __tetromino__ closely touch from below other __tetromino__,
it will stop, and next tetramino will appear.
If stopped __tetrominos__ create horizontal line full filled with squares,
then it will disappear.
If column of __tetrominos__ will reach upper bound of __game map__,
then game will over.
![__tetromino__ types](./assets/tetromino-types.png)


__game map__
  - consist of squares WxH 10x20 
  - at button start pressed first __tetromino__ will spawn on the top
 
  



 
