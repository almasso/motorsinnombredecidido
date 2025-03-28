local Move = require('assets.events.moveEventBehaviour')

return {
    music = {
        components = {
            AudioSource = {
                clip = "assets/audio/audio.wav",
                mixer = "assets/audio/music.mixer.lua",
                volume = 0.8,
                loop = true,
                playOnAwake = true
            },
            Animator = {
                animation = "assets/sprites/test.animation.lua",
                playing = true
            },
            Transform = {
                pos = {0, 0}
            },
            Camera = {
                size = {200 , 200}
            },
            EventHandler = {
                events = {
                    Evento1 = {
                        condition = {
                            type = "Or",
                            params = {
                                conditionA = {
                                    type = "OnStart"
                                },
                                conditionB = {
                                    type = "CollidedWith",
                                    params = {
                                        other = "test"
                                    }
                                },
                            }
                        },
                        behaviours = {
                            Move:new()
                        }
                    }
                }
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