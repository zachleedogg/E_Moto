/****************************************************************************
* Title                 :   Wachdog Timer 
* Filename              :   wdt.c
* Author                :   Jacob Beningo
* Origin Date           :   08/19/2015
* Version               :   1.0.0
* Compiler              :   Keil uVision v5.14.0
* Target                :   STM32L0R53
* Notes                 :   None
*
* THIS SOFTWARE IS PROVIDED BY BENINGO ENGINEERING "AS IS" AND ANY EXPRESSED
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL BENINGO ENGINEERING OR ITS CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/
/****************************************************************************
* Doxygen C Template
* Copyright (c) 2013 - Jacob Beningo - All Rights Reserved
*
* Feel free to use this Doxygen Code Template at your own risk for your own
* purposes.  The latest license and updates for this Doxygen C template can be  
* found at www.beningo.com or by contacting Jacob at jacob@beningo.com.
*
* For updates, free software, training and to stay up to date on the latest 
* embedded software techniques sign-up for Jacobs newsletter at 
* http://www.beningo.com/814-2/
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Template. 
*
*****************************************************************************/
/*************** SOURCE REVISION LOG *****************************************
*
*    Date    Version   Author         Description 
*  08/19/15   1.0.0   Jacob Beningo   Initial Release.
*
*******************************************************************************/
/** @file wdt.c
 *  @brief This is the source file for watchdog timer driver
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>				/* For portable types */
#include "wdt.h"				  /* For definitions and types */		
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/**
 * Defines the default maximum refresh count to load.
 */
static WdtCounter_t WdtRefreshCount = WDT_COUNT_MAX;

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function : Wdt_Init()
*//** 
* \b Description:
*
*  This function is used to initialize the watchdog timer.
*
* @param  		Config - a pointer to the watchdog configuration structure.
*
* @return 		void
*
* @see Wdt_Init
* @see Wdt_Enable
* @see Wdt_Disable
* @see Wdt_Reset
* @see Wdt_Clear
*
* \b Example Example:
* @code
*    Wdt_Init();
* @endcode
*
*  ----------------------
*  - HISTORY OF CHANGES -
*  ----------------------
*    Date    Version   Author         Description 
*  08/19/15   1.0.0   Jacob Beningo   Initial Release.
*
*******************************************************************************/
void Wdt_Init(const WdtConfig_t * Config)
{
   /* Enable the debug register clock? */
	 if(Config->DebugState == WDT_DEBUG_ENABLED)
	 {		 
      /* Enable the peripheral clock of DBG register */
      RCC->APB2ENR |= RCC_APB2ENR_DBGMCUEN;
      DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_WWDG_STOP; /* To be able to debug */
	 }
  
   /* Enable the peripheral clock WWDG */
   RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;

	 WWDG->CFR = Config->WindowCount;
	 WWDG->CR = Config->CounterValue;	 
	 
	 WdtRefreshCount = Config->CounterValue;
}

/******************************************************************************
* Function : Wdt_Enable()
*//** 
* \b Description:
*
*  This function is used to enable the watchdog.  Usually done after system 
*  initialization.
*
* @return 		void
*
* @see Wdt_Init
* @see Wdt_Enable
* @see Wdt_Disable
* @see Wdt_Reset
* @see Wdt_Clear
*
* \b Example Example:
* @code
*    Wdt_Enable();
* @endcode
*
*  ----------------------
*  - HISTORY OF CHANGES -
*  ----------------------
*    Date    Version   Author         Description 
*  08/19/15   1.0.0   Jacob Beningo   Initial Release.
*
*******************************************************************************/
void Wdt_Enable(void)
{
	WWDG->CR |= WWDG_CR_WDGA;
}

/******************************************************************************
* Function : Wdt_Disable()
*//** 
* \b Description:
*
*  This function is used to disable the watchdog.  Something that is never 
*  recommended but alas, here is the ability to do it.
*
* @return 		void
*
* @see Wdt_Init
* @see Wdt_Enable
* @see Wdt_Disable
* @see Wdt_Reset
* @see Wdt_Clear
*
* \b Example Example:
* @code
*    // Should never do this! 
*    Wdt_Disable();
* @endcode
*
*  ----------------------
*  - HISTORY OF CHANGES -
*  ----------------------
*    Date    Version   Author         Description 
*  08/19/15   1.0.0   Jacob Beningo   Initial Release.
*
*******************************************************************************/
void Wdt_Disable(void)
{
   // CANNOT BE DISABLED ON THIS PART :-D !!
}

/******************************************************************************
* Function : Wdt_Reset()
*//** 
* \b Description:
*
*  This function is used to force a watchdog reset.  Reloading the counter 
*  outside of the normal window will result in a reset.  
*
* @return 		void
*
* @see Wdt_Init
* @see Wdt_Enable
* @see Wdt_Disable
* @see Wdt_Reset
* @see Wdt_Clear
*
* \b Example Example:
* @code
*    Wdt_Reset();
* @endcode
*
*  ----------------------
*  - HISTORY OF CHANGES -
*  ----------------------
*    Date    Version   Author         Description 
*  08/19/15   1.0.0   Jacob Beningo   Initial Release.
*
*******************************************************************************/
void Wdt_Reset(void)
{
	/* Just in case we accidentally try to force reset in the window. */
	for(;;)
	{
		WWDG->CR = WdtRefreshCount; 
	}
}

/******************************************************************************
* Function : Wdt_Clear()
*//** 
* \b Description:
*
*  This function is used to pet the watchdog.
*
* @return 		void
*
* @see Wdt_Init
* @see Wdt_Enable
* @see Wdt_Disable
* @see Wdt_Reset
* @see Wdt_Clear
*
* \b Example Example:
* @code
*    Wdt_Clear();
* @endcode
*
*  ----------------------
*  - HISTORY OF CHANGES -
*  ----------------------
*    Date    Version   Author         Description 
*  08/19/15   1.0.0   Jacob Beningo   Initial Release.
*
*******************************************************************************/
void Wdt_Clear(void)
{
	WWDG->CR = WdtRefreshCount; 
}

/*************** END OF FUNCTIONS ***************************************************************************/
