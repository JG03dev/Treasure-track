static SoundEffectsPlayer effectsPlayer1;
static SoundEffectsPlayer effectsPlayer2;
static SoundEffectsPlayer effectsPlayer3;
static SoundEffectsPlayer effectsPlayer4;
static uint32_t sound1 = SE_LOAD("../../../music/sounds/arranque.wav");
static uint32_t sound2 = SE_LOAD("../../../music/sounds/acelerar2.wav");
static uint32_t sound3 = SE_LOAD("../../../music/sounds/frenar.wav");
static uint32_t sound4 = SE_LOAD("../../../music/sounds/acelerar1.wav");
static uint32_t sound5 = SE_LOAD("../../../music/sounds/motor_encendido.wav");
static uint32_t sound6 = SE_LOAD("../../../music/sounds/nitro.wav");
static uint32_t sound7 = SE_LOAD("../../../music/sounds/pre-aceleracion.wav");

static MusicBuffer myMusic("../../../music/coconut.wav");
static bool wasWPressed = false;
static bool once = true;
// Rendering loop
while (!glfwWindowShouldClose(gWindow)) {

	myMusic.UpdateBufferStream();

	// Aqui va el resto del codigo

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
	else if (musiccontrolcooldown > 0 && GetKeyState('Z') & 0x8000)
	{
		if (myMusic.isPlaying())
		{
			myMusic.Pause();

			static MusicBuffer myMusic("../../../music/cars.wav");
			myMusic.Play();
		}
		else
		{
			static MusicBuffer myMusic("../../../music/cars.wav");
		}
		_sleep(100);
		musiccontrolcooldown = 0;
	}

	MainLoop::Get()->AddToDelayedUpdate([]() {
		if (myMusic.isPlaying())
		{
			myMusic.UpdateBufferStream();
		}
		});

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	glfwPollEvents();
	glfwSwapBuffers(gWindow);
}

glfwTerminate();

return 0;
}