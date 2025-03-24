return {
    music = {
        components = {
            AudioSource = {
                clip = "assets/audio/audio.wav",
                mixer = "assets/audio/music.mixer.lua",
                volume = 0.8,
                loop = true,
            }
        }
    },
    sfx = {
        components = {
            AudioSource = {
                enabled = false,
                clip = "assets/audio/SodaLoop.wav",
                mixer = "assets/audio/sfx.mixer.lua",
                volume = 1,
                loop = false
            }
        }
    }
};