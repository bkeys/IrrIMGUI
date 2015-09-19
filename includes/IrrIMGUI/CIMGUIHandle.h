/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Andre Netzeband
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**
 * @file   CIMGUIHandle.h
 * @author Andre Netzeband
 * @brief  Contains a handle to setup the IMGUI for Irrlicht.
 * @addtogroup IrrIMGUI
 */

#ifndef IRRIMGUI_INCLUDE_IRRIMGUI_CIMGUIHANDLE_H_
#define IRRIMGUI_INCLUDE_IRRIMGUI_CIMGUIHANDLE_H_

// library includes
#include <IrrIMGUI/IrrIMGUI.h>
#include <IrrIMGUI/CIMGUIEventStorage.h>
#include <Irrlicht/irrlicht.h>

// module includes
#include "SIMGUISettings.h"

/**
 * @addtogroup IrrIMGUI
 * @{
 */
namespace IrrIMGUI
{
  // forward declaration
  namespace Private
  {
    class IIMGUIDriver;
  }

  /**
   * @brief Use an object of this class to setup the IMGUI for Irrlicht and to render the content.
   * @details
   *   Create an object of this handle in your project, when you need the IMGUI. You can create multiple object, but all
   *   of them will just use a single instance of the GUI (this is an IMGUI limitation). You can setup and use the same GUI
   *   from all instances of this handle.
   *
   *   If the last instance is destroyed, the IMGUI system will be shutdown.
   *
   *   To create an instance of this handle you need to pass an Irrlicht device object to the constructor.
   *   The Event-Storage pointer can be an "nullptr" or NULL, if you don't need mouse or keyboard input for your GUI.
   *
   *   To draw a GUI add the corresponding IMGUI element functions to your main loop between calling "startGUI()" and
   *   "drawAll()" of this class.
   *
   *   @code

  IrrIMGUI::CIMGUIHandle GUI(pDevice, &EventReceiver);

  irr::scene::ISceneManager * const pSceneManager   = pDevice->getSceneManager();

  // irrlicht main loop
  while(pDevice->run())
  {
    pDriver->beginScene(true, true, irr::video::SColor(255,100,101,140));

    // drawing the GUI
    GUI->startGUI();
    ImGui::Text("Hello, world!");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // render the scene
    pSceneManager->drawAll();

    // render the GUI
    mpGUI->drawAll();

    pDriver->endScene();
  }

   @endcode
   *
   */
  class CIMGUIHandle
  {
    public:

      /// @{
      /// @name Constructor and Destructor

      /**
       * @brief Use this constructor when you want special settings.
       * @note  When you use multiple instances of this handle class, the settings will be applied to all instances at the same time.
       *
       * @param pDevice       Is a pointer to the Irrlicht Device.
       * @param pEventStorage Is a pointer to the Event Storage that is used to transfer Mouse and Key input informations to the IMGUI. If you pass NULL or nullptr, no input informations are passed to the GUI.
       * @param rSettings     Is a reference to the a Settings structure.
       */
      CIMGUIHandle(irr::IrrlichtDevice * pDevice, CIMGUIEventStorage * pEventStorage, SIMGUISettings const &rSettings);

      /**
       * @brief Use this constructor when you don't want to change the settings (using defualt settings or the settings that have been applied last time).
       *
       * @param pDevice       Is a pointer to the Irrlicht Device.
       * @param pEventStorage Is a pointer to the Event Storage that is used to transfer Mouse and Key input informations to the IMGUI. If you pass NULL or nullptr, no input informations are passed to the GUI.
       */
      CIMGUIHandle(irr::IrrlichtDevice * pDevice, CIMGUIEventStorage * pEventStorage);

      /// @brief Destructor
      ~CIMGUIHandle(void);

      /// @}

      /// @{
      /// @name Render and drawing methods

      /// @brief Call this methods before you draw the IMGUI elements and before calling "drawAll()".
      void startGUI(void);

      /// @brief Call this function after "startGUI()" and after you draw your GUI elements. It will render all elements to the screen (do not call it before rendering the 3D Scene!).
      void drawAll(void);

      /// @}

      /// @{
      /// @name GUI settings

      /// @return Returns a constant reference to the currently applied settings structure.
      SIMGUISettings const &getSettings(void) const;

      /// @param rSettings is a reference to a Setting structure that should be applied.
      /// @note  The settings are applied to all GUI handles at the same time, since IMGUI uses internally a single instance.
      void setSettings(SIMGUISettings const &rSettings);

      /// @}

      /// @{
      /// @name Font operations

      /**
       * @brief Adds a font to the IMGUI memory.
       * @param pFontConfig Is a pointer to the font configuration.
       * @return Returns a pointer to the font for later usage with PushFont(...) to activate this font.
       */
      ImFont * addFont(ImFontConfig const * pFontConfig);

      /**
       * @brief Adds the default font to the IMGUI memory.
       * @param pFontConfig Is a pointer to the font configuration.
       * @return Returns a pointer to the font for later usage with PushFont(...) to activate this font.
       */
      ImFont * addDefaultFont(ImFontConfig const * pFontConfig = NULL);

      /**
       * @brief Adds a font from a TTF file to the IMGUI memory.
       * @param pFileName       Is the name of the file to add.
       * @param FontSizeInPixel Is the desired font size to use.
       * @param pFontConfig     Is a pointer to the font configuration.
       * @param pGlyphRanges    Is the Glyph-Range to select the correct character set.
       * @return Returns a pointer to the font for later usage with PushFont(...) to activate this font.
       */
      ImFont * addFontFromFileTTF(char const * pFileName, float FontSizeInPixel, ImFontConfig const * pFontConfig = NULL, ImWchar const * pGlyphRanges = NULL);

      /**
       * @brief Adds a font from a TTF byte array to the IMGUI memory.
       * @param pTTFData        Is a pointer to the byte array.
       * @param TTFSize         Is the size of the array in byte.
       * @param FontSizeInPixel Is the desired font size to use.
       * @param pFontConfig     Is a pointer to the font configuration.
       * @param pGlyphRanges    Is the Glyph-Range to select the correct character set.
       * @return Returns a pointer to the font for later usage with PushFont(...) to activate this font.
       *
       * @attention This function transfers the ownership of 'pTTFData' to the IMGUI System and will be deleted automatically. Do not delete it by yourself!
       */
      ImFont * addFontFromMemoryTTF(void * pTTFData, int TTFSize, float FontSizeInPixel, ImFontConfig const * pFontConfig = NULL, ImWchar const * pGlyphRanges = NULL);

      /**
       * @brief Adds a font from a compressed TTF byte array to the IMGUI memory.
       * @param pCompressedTTFData Is a pointer to the byte array.
       * @param CompressedTTFSize  Is the size of the array in byte.
       * @param FontSizeInPixel    Is the desired font size to use.
       * @param pFontConfig        Is a pointer to the font configuration.
       * @param pGlyphRanges       Is the Glyph-Range to select the correct character set.
       * @return Returns a pointer to the font for later usage with PushFont(...) to activate this font.
       *
       * @note This function does not transfer the ownership of the byte array. You are responsible for delete this memory after the font is in graphic memory.
       */
      ImFont * addFontFromMemoryCompressedTTF(void const * pCompressedTTFData, int CompressedTTFSize, float FontSizeInPixel, ImFontConfig const * pFontConfig = NULL, ImWchar const * pGlyphRanges = NULL);

      /**
       * @brief Adds a font from a compressed TTF byte array that uses the base85 character encoding to the IMGUI memory.
       * @param pCompressedTTFDataBase85 Is a pointer to the char array.
       * @param FontSizeInPixel          Is the desired font size to use.
       * @param pFontConfig              Is a pointer to the font configuration.
       * @param pGlyphRanges             Is the Glyph-Range to select the correct character set.
       * @return Returns a pointer to the font for later usage with PushFont(...) to activate this font.
       *
       * @note This function does not transfer the ownership of the byte array. You are responsible for delete this memory after the font is in graphic memory.
       */
      ImFont * addFontFromMemoryCompressedBase85TTF(char const * pCompressedTTFDataBase85, float FontSizeInPixel, ImFontConfig const * pFontConfig = NULL, const ImWchar * pGlyphRanges = NULL);

      /// @brief This function copies all fonts that have been added with "addFont/addDefaultFont" into graphic memory.
      /// @attention Call this function before using the fonts that have been added.
      void compileFonts(void);

      /// @brief Resets the font memory and restores the default font as the one and only font in the system.
      void resetFonts(void);

      /// @}


      /// @{
      /// @name Common Font Glyph-Ranges

      /// @return Returns the Basic Latin and Extended Latin Range.
      ImWchar const * getGlyphRangesDefault(void);

      /// @return Returns the Default + Hiragana, Katakana, Half-Width, Selection of 1946 Ideographs
      ImWchar const * getGlyphRangesJapanese(void);

      /// @return Returns the Japanese + full set of about 21000 CJK Unified Ideographs
      ImWchar const * getGlyphRangesChinese(void);

      /// @return Default + about 400 Cyrillic characters
      ImWchar const * getGlyphRangesCyrillic(void);

      /// @}

      /// @{
      /// @name Image and Texture methods

      /**
       * @brief Creates an texture ID out of a Irrlicht image. This texture ID can be used draw images with ImGui::Image function.
       * @param pImage Is a pointer to an IImage object. After the texture has been created, the IImage object can be destroyed.
       * @return Returns an texture ID that can be used to draw images. To delete the texture from graphic memory use later deleteTexture(...)
       */
      ImTextureID createTextureFromImage(irr::video::IImage * pImage);

      /**
       * @brief Deletes an texture from graphic memory.
       * @param Texture Is the texture to delete. Do not use it with ImGui::Image(..) function afterwards.
       */
      void deleteTexture(ImTextureID Texture);

      /// @}

    private:
      /// @brief Updates the screen size used for IMGUI
      void updateScreenSize(void);

      /// @brief Updated the IMGUI timers
      void updateTimer(void);

      /// @brief Update Mouse input information.
      void updateMouse(void);

      /// @brief Update Keyboard input information.
      void updateKeyboard(void);

      Private::IIMGUIDriver * mpGUIDriver;
      irr::f32                mLastTime;
      CIMGUIEventStorage    * mpEventStorage;
      static irr::u32         mHandleInstances;

  };

}
/**
 * @}
 */

#endif /* IRRIMGUI_INCLUDE_IRRIMGUI_CIMGUIHANDLE_H_ */
