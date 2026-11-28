#pragma once

/**
 * This a dedicated module for drawing sprites
 * 
 * Sprites are Records containing 256 bytes (16x16 px)
 * 
 * When it comes to draw sprite:
 * 
 *      The draw might be done in C++, with a pullRect() -> pixel color settings -> pushRect()
 *
 *      For the API, we must make a choice between 2 ways:
 *      -> draw_sprite(spritename: str, x: int, y: int)
 *      -> sprite = Sprite(spritename: str)
 *         sprite.move(x: int, y: int)
 *         sprite.draw()
 * 
 *      The second way seems better because the pixels ptr of sprite would
 *      be kept all the programm, so fetching memory 1 time
 * 
 *      But we are not sure that the pixels will not be moved in memory...
 *      So maybe the first way is safest but slower...
 */
