#include "MySoundEffects.h"

MySoundEffects::MySoundEffects() : wasWPressed(false), once(true) {
    sound1 = SE_LOAD("../../../music/sounds/arranque.wav");
    sound2 = SE_LOAD("../../../music/sounds/acelerar2.wav");
    sound3 = SE_LOAD("../../../music/sounds/frenar.wav");
    sound4 = SE_LOAD("../../../music/sounds/acelerar1.wav");
    sound5 = SE_LOAD("../../../music/sounds/motor_encendido.wav");
    sound6 = SE_LOAD("../../../music/sounds/nitro.wav");
    sound7 = SE_LOAD("../../../music/sounds/pre-aceleracion.wav");
}

void MySoundEffects::PlaySound(float deltaTime) {
  
	    float healcooldown = 3;
	    healcooldown += deltaTime;
	    float failcooldown = .8f;
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
		    effectsPlayer1.Play(sound5);
		    wasWPressed = false;
	    }

	    if (GetKeyState(VK_SHIFT) & 0x8000)
	    {
		    effectsPlayer4.Stop();
		    effectsPlayer4.Play(sound6);
		    healcooldown = 0;
		    failcooldown = 0;
	    }
}