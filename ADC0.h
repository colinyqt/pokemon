// /* ADC0.h
//  * Students put your names here
//  * Modified: put the date here
//  * 12-bit ADC input on ADC0 channel ____, 
//  */

 /**
  * @file      ADC0.h
  * @brief     12-bit ADC input
  * @details   12-bit ADC input on ADC0 channel 5, PB18<br>
  * @version   ECE319K Spring 2024
  * @author    Your names go here
  * @copyright Lab solution, do not post,
  * @warning   AS-IS
  * @note      For more information see  http://users.ece.utexas.edu/~valvano/
  * @date      December 29, 2023
   <table>
 <caption id="Lab7pins">Pins for the 12-bit ADC</caption>
 <tr><th>Pin <th>GPIO<th>Hardware
 <tr><td>PB18 <td>input<td>connected to slide-pot sensor
 </table>
   ******************************************************************************/
 #ifndef __ADC0_H__
 #define __ADC0_H__

 /**
  * Initialize 12-bit ADC
  *    - PB18 analog input
  * @param none
  * @return none
  * @brief  Initialize ADC
  * @note LaunchPad_Init has been called; this program should not reset Port B
  */
 void ADC0init(void);
 // write code to initialize ADC0
 // Your measurement will be connected to PB18
 // 12-bit mode, 0 to 3.3V, right justified
 // software trigger, no averaging

 /**
  * Input from 12-bit ADC
  * @param none
  * @return sample 12-bit conversion
  * @brief  Input from PB18
  */
 uint32_t ADC0xin(void);
   // write code to sample ADC0
   // return digital result (0 to 4095)

uint32_t ADCyin(void);


#endif // __ADC0_H__
