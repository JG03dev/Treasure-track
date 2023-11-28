#pragma once

#pragma region ENCABEZADOS

#include "../Encabezados/stdafx.h" //si no hace borrar

#pragma endregion

#pragma region SONIDOS

#include "MusicBuffer.h"
#include "MusicBuffer.h"
#include "SoundEffectsLibrary.h"
#include "SoundEffectsPlayer.h"

#pragma endregion

class MySoundEffects {
public:
    // Public Attributes


	// Constructors
	MySoundEffects();

    void PlaySound(float deltaTime);

private:

	// Private Attributes
    SoundEffectsPlayer effectsPlayer1;
    SoundEffectsPlayer effectsPlayer2;
    SoundEffectsPlayer effectsPlayer3;
    SoundEffectsPlayer effectsPlayer4;

    uint32_t sound1, sound2, sound3, sound4, sound5, sound6, sound7;

    //MusicBuffer myMusic;
    bool wasWPressed;
    bool once;
};