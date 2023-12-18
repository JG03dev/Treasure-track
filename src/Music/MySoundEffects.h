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
    MySoundEffects() : wasWPressed(false), once(true), myMusic("../../../music/music1.wav") {
        sound1 = SE_LOAD("../../../music/sounds/arranque.wav");
        sound2 = SE_LOAD("../../../music/sounds/acelerar2.wav");
        sound3 = SE_LOAD("../../../music/sounds/frenar.wav");
        sound4 = SE_LOAD("../../../music/sounds/acelerar1.wav");
        sound5 = SE_LOAD("../../../music/sounds/motor_encendido.wav");
        sound6 = SE_LOAD("../../../music/sounds/nitro.wav");
        sound7 = SE_LOAD("../../../music/sounds/pre-aceleracion.wav");
        sound8 = SE_LOAD("../../../music/sounds/MairoCoinSound.wav");
        sound9 = SE_LOAD("../../../music/sounds/choque.wav");
        myMusic.Play();
        num = 1;
        musicaTotal = 4 + 1; // Number of songs + empty song
        shiftPressedPreviousFrame = false;
    }

    void PlayCoinSound() {
        if (effectsPlayer5.isPlaying())
            effectsPlayer5.Stop();

        effectsPlayer5.Play(sound8);
    }

    void Choque() {
        if (effectsPlayer5.isPlaying())
            effectsPlayer5.Stop();

        effectsPlayer5.Play(sound9);
    }

    void PlaySound(float deltaTime) {
        static float healcooldown = 3;
        healcooldown += deltaTime;
        static float failcooldown = .8f;
        failcooldown += deltaTime;

        bool shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
        if (!shiftPressedPreviousFrame && shiftPressed) {
            effectsPlayer4.Stop();
            effectsPlayer4.Play(sound6);
            healcooldown = 0;
            failcooldown = 0;
        }
        shiftPressedPreviousFrame = shiftPressed;

        if (GetKeyState('W') & 0x8000)
        {
            if (healcooldown > 3) {
                if (!wasWPressed && !once) {
                    effectsPlayer1.Stop();
                    effectsPlayer1.Play(sound4);
                    wasWPressed = true;
                    healcooldown = 0;
                    failcooldown = 0;
                    once = false;
                }
                if (!wasWPressed && once) {
                    effectsPlayer1.Stop();
                    effectsPlayer1.Play(sound1);
                    effectsPlayer3.Stop();
                    effectsPlayer3.Play(sound7);
                    wasWPressed = true;
                    healcooldown = 0;
                    failcooldown = 0;
                    once = false;
                }
                else {
                    if (!effectsPlayer2.isPlaying() && failcooldown > .8f) {
                        effectsPlayer2.Play(sound2);
                        effectsPlayer1.Stop();
                        healcooldown = 0;
                        failcooldown = 0;
                    }
                }
            }
        }
        else if (wasWPressed) {
            effectsPlayer3.Stop();
            effectsPlayer3.Play(sound3);
            effectsPlayer2.Stop();
            effectsPlayer1.Stop();
            effectsPlayer1.Play(sound5);
            wasWPressed = false;
        }
    }

    void PlayMusic(float deltaTime) 
    {
        myMusic.UpdateBufferStream();
        
        static float cooldown = 0.0f;
        const float cooldownTime = 1.0f;
        cooldown -= deltaTime;

        if (GetKeyState('Q') & 0x8000 && cooldown <= 0.0f) 
        {
            num = (num + 1) % (musicaTotal);

            if (num < musicaTotal - 1) 
            {
                std::string rutaMusica = "../../../music/music" + std::to_string(num) + ".wav";
                myMusic.Reset(rutaMusica.c_str());
                myMusic.Play();
            }
            else 
            {
                if (myMusic.isPlaying()) 
                {
                    myMusic.Pause();
                }
            }
            cooldown = cooldownTime;
        }

        if (GetKeyState('E') & 0x8000 && cooldown <= 0.0f) 
        {
            num = num - 1;
            num = ((num % musicaTotal) + musicaTotal) % musicaTotal;

            if (num < musicaTotal - 1)
            {
                std::string rutaMusica = "../../../music/music" + std::to_string(num) + ".wav";
                myMusic.Reset(rutaMusica.c_str());
                myMusic.Play();
            }
            else 
            {
                if (myMusic.isPlaying()) 
                {
                    myMusic.Pause();
                }
            }
            cooldown = cooldownTime;
        }
        cooldown -= deltaTime;
        cooldown = std::max(cooldown, 0.0f);
    }



private:
    SoundEffectsPlayer effectsPlayer1;
    SoundEffectsPlayer effectsPlayer2;
    SoundEffectsPlayer effectsPlayer3;
    SoundEffectsPlayer effectsPlayer4;
    SoundEffectsPlayer effectsPlayer5;

    uint32_t sound1, sound2, sound3, sound4, sound5, sound6, sound7, sound8, sound9;

    bool wasWPressed;
    bool once;
    MusicBuffer myMusic;
    int num;
    int musicaTotal;
    bool shiftPressedPreviousFrame;
    
};
