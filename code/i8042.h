#ifndef __I8042_H
#define __I8042_H

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042.
 */


#define BIT(n) (0x01<<(n))						/**< @brief How to refer to a determined BIT */

#define DELAY_US		20000					/**< @brief time to delay */

#define QQ				3
#define WAIT_KBC		1000

#define KBD_IRQ	        1			    		/**< @brief KEYBOARD IRQ line */
#define MOUSE_IRQ	    12			    		/**< @brief MOUSE IRQ line */
#define KBD_ESC_REL 	0x81					/**< @brief KEYBOARD ESCAPE RELEASE */
#define OUT_BUF 		0x60					/**< @brief KEYBOARD OUT BUFFER register */
#define STAT_REG		0x64					/**< @brief KEYBOARD STATUS register */
#define LED_SWITCHES	0xED					/**< @brief KEYBOARD LED SWITCHES register */
#define RESEND			0xFE					/**< @brief KEYBOARD code to resend */
#define ERROR			0xFC					/**< @brief KEYBOARD code of error */
#define ACK				0xFA					/**< @brief KEYBOARD ACK */
#define KBC_CMD_REG		0x64					/**< @brief KBC command register */
#define WORD			0xE0					/**< @brief WORD */
#define MOUSE_ACT		0xA8					/**< @brief MOUSE ACTIVATE */

#define IBF				BIT(1)
#define OBF				BIT(0)
#define PAR_ERR			BIT(7)
#define TO_ERR			BIT(5)
#define LEDS_ON			BIT(0)|BIT(1)|BIT(2)
#define RESEND			0xFE
#define ERROR			0xFC

/**@}*/


#endif
