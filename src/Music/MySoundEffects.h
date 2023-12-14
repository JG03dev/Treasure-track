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
    MySoundEffects() : wasWPressed(false), once(true), myMusic("../../../music/coconut.wav") {
        sound1 = SE_LOAD("../../../music/sounds/arranque.wav");
        sound2 = SE_LOAD("../../../music/sounds/acelerar2.wav");
        sound3 = SE_LOAD("../../../music/sounds/frenar.wav");
        sound4 = SE_LOAD("../../../music/sounds/acelerar1.wav");
        sound5 = SE_LOAD("../../../music/sounds/motor_encendido.wav");
        sound6 = SE_LOAD("../../../music/sounds/nitro.wav");
        sound7 = SE_LOAD("../../../music/sounds/pre-aceleracion.wav");
        sound8 = SE_LOAD("../../../music/sounds/MairoCoinSound.wav");
    }

    void PlayCoinSound() {
        if (effectsPlayer5.isPlaying())
            effectsPlayer5.Stop();

        effectsPlayer5.Play(sound8);
    }

    void PlaySound(float deltaTime) {
        static float healcooldown = 3;
        healcooldown += deltaTime;
        static float failcooldown = .8f;
        failcooldown += deltaTime;

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

        if (GetKeyState(VK_SPACE) & 0x8000)
        {
            effectsPlayer4.Stop();
            effectsPlayer4.Play(sound6);
            healcooldown = 0;
            failcooldown = 0;
        }
    }

    void PlayMusic(float deltaTime) {
        myMusic.UpdateBufferStream();

        static float musiccontrolcooldown = 1;
        musiccontrolcooldown += deltaTime;
        if (musiccontrolcooldown > 0 && GetKeyState('Q') & 0x8000)
        {
            if (myMusic.isPlaying())
            {
                myMusic.Pause();
            }
            else
            {
                myMusic.Play();
            }
            _sleep(100);
            musiccontrolcooldown = 0;
        }
    }

private:
    SoundEffectsPlayer effectsPlayer1;
    SoundEffectsPlayer effectsPlayer2;
    SoundEffectsPlayer effectsPlayer3;
    SoundEffectsPlayer effectsPlayer4;
    SoundEffectsPlayer effectsPlayer5;

    uint32_t sound1, sound2, sound3, sound4, sound5, sound6, sound7, sound8;

    bool wasWPressed;
    bool once;
    MusicBuffer myMusic;
};
