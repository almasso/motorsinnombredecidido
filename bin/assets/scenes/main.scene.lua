local Move = require('assets.events.MoveBehaviour');
local WaitFor = require('assets.events.WaitForBehaviour');
local Jump = require('assets.events.JumpBehaviour');
local JumpIf = require('assets.events.JumpIfBehaviour');
local PlaySFX = require('assets.events.PlaySFXBehaviour');
local Music = require('assets.events.MusicBehaviour');
local Animation = require('assets.events.AnimationBehaviour');
local ModifyLocalVariable = require('assets.events.ModifyLocalVariableBehaviour');

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
    entity1 = {
        children = {
            sfx1 = {
                handler = "entity1_sfx1",
                components = {
                    AudioSource = {
                        clip = "assets/audio/SodaLoop.wav",
                        mixer = "assets/audio/sfx.mixer.lua",
                        volume = 1.0,
                        loop = false
                    }
                },
            },
        },
        components = {
            Animator = {
                animation = "assets/sprites/test.animation.lua",
                playing = true
            },
            Transform = {
                pos = {0, 0}
            },
            MovementComponent = {
                speed = 500
            },
            PlayerInput = {0},
            LocalVariables = { counter = 1 },
            EventHandler = {
                events = {
                    Evento0 = {
                        loop = false,
                        condition = {
                            type = "OnStart"
                        },
                        behaviours = {
                            Animation:new({action = "play"}),
                            ModifyLocalVariable:new("counter", 2),
                            JumpIf:new(0, { type = "ValueEquals", params = { variable = "counter", equals = 1 }}),
                            Animation:new({action = "change", animation = "assets/sprites/test2.animation.lua"})
                        }
                    },
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
                            Music:new({action = "play"}),
                            WaitFor:new({type = "TimePassed", params = { seconds = 1.0 }}),
                            Move:new(0, 200),
                            WaitFor:new({type = "BehaviourEnded", params = { behaviour = 2 }}),
                            Move:new(200, 200),
                            WaitFor:new({type = "BehaviourEnded", params = { behaviour = 4 }}),
                            Move:new(200, 0),
                            WaitFor:new({type = "BehaviourEnded", params = { behaviour = 6 }}),
                            Move:new(0, 0),
                            WaitFor:new({type = "BehaviourEnded", params = { behaviour = 8 }}),
                            JumpIf:new(15, {type = "BehaviourEnded", params = { behaviour = 14 }}),
                            PlaySFX:new("entity1_sfx1"),
                            Music:new({action = "stop"}),
                            Music:new({action = "change", clip="assets/audio/SodaLoop.wav"}),
                            Jump:new(0),
                            JumpIf:new(1, {type = "BehaviourEnded", params = { behaviour = 17 }}),
                            Music:new({action = "change", clip="assets/audio/audio.wav"}),
                            Music:new({action = "volume", volume = 0.5})
                        }
                    }
                }
            }
        }
    },
    music = {
        handler = "Music",
        components = {
            AudioSource = {
                clip = "assets/audio/audio.wav",
                mixer = "assets/audio/music.mixer.lua",
                volume = 1,
                loop = true
            }
        }
    }
};