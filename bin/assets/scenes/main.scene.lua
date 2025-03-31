local Move = require('assets.events.MoveBehaviour');
local WaitFor = require('assets.events.WaitForBehaviour');

return {
    manager = {
        handler = "Manager",
        components = {
            MovementManager = {
                tileWidth = 200,
                tileHeight =200
            },
            Transform = {0},
            Camera = {
                size = {1920,1080}
            }
        }
    },
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
            MovementComponent = {
                speed = 100
            },
            PlayerInput = {0},
            EventHandler = {
                events = {
                    Evento1 = {
                        loop = true,
                        condition = {
                            type = "Or",
                            params = {
                                conditionA = {
                                    type = "OnStart"
                                },
                                conditionB = {
                                    type = "OnStart"
                                },
                            }
                        },
                        behaviours = {
                            WaitFor:new({type = "TimePassed", params = { seconds = 0.5 }}),
                            Move:new(0, 200),
                            WaitFor:new({type = "BehaviourEnded", params = { behaviour = 1 }}),
                            Move:new(200, 200),
                            WaitFor:new({type = "BehaviourEnded", params = { behaviour = 3 }}),
                            Move:new(200, 0),
                            WaitFor:new({type = "BehaviourEnded", params = { behaviour = 5 }}),
                            Move:new(0, 0)
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