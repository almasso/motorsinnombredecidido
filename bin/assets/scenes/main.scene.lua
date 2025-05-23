local Move = require('data.events.MoveBehaviour');
local WaitFor = require('data.events.WaitForBehaviour');
local Jump = require('data.events.JumpBehaviour');
local JumpIf = require('data.events.JumpIfBehaviour');
local PlaySFX = require('data.events.PlaySFXBehaviour');
local Music = require('data.events.MusicBehaviour');
local Animation = require('data.events.AnimationBehaviour');
local ModifyVariable = require('data.events.ModifyVariableBehaviour');
local Dialogue = require('data.events.DialogueBehaviour')
local Choices = require('data.events.ChoicesBehaviour')

return {
    manager = {
        handler = "Manager",
        children = {
            textbox = {
                handler = "TextBox",
                active = false,
                components = {
                    Transform = {
                        position = {0, 370}
                    },
                    Text = {
                        font = "assets/fonts/pokemon-emerald.ttf",
                        fontSize = 64,
                        color = 0x000000FF,
                        size = {1580,280},
                        layer = 2
                    },
                    Rectangle = {
                        layer = 1,
                        size = {1600,290},
                        color = 0xFFFFFFFF
                    },
                    TextBox = {0}
                }
            },
            choice1 = {
                handler = "Choice1",
                active = false,
                components = {
                    Transform = {
                        position = {-400, 100}
                    },
                    Text = {
                        text = "Sí",
                        font = "assets/fonts/pokemon-emerald.ttf",
                        fontSize = 64,
                        color = 0x000000FF,
                        size = {200,100},
                        centerText = true,
                        layer = 2
                    },
                    Rectangle = {
                        layer = 1,
                        size = {220,120},
                        color = 0xFFFFFFFF
                    },
                    Button = {
                        size = {220,120},
                        params = {2}
                    }
                }
            },
            choice2 = {
                handler = "Choice2",
                active = false,
                components = {
                    Transform = {
                        position = {0, 100}
                    },
                    Text = {
                        text = "No",
                        font = "assets/fonts/pokemon-emerald.ttf",
                        fontSize = 64,
                        color = 0x000000FF,
                        size = {200,100},
                        centerText = true,
                        layer = 2
                    },
                    Rectangle = {
                        layer = 1,
                        size = {220,120},
                        color = 0xFFFFFFFF
                    },
                    Button = {
                        size = {220,120},
                        params = {2}
                    }
                }
            },
            choice3 = {
                handler = "Choice3",
                active = false,
                components = {
                    Transform = {
                        position = {400, 100}
                    },
                    Text = {
                        text = "¿Huh?",
                        font = "assets/fonts/pokemon-emerald.ttf",
                        fontSize = 64,
                        color = 0x000000FF,
                        size = {200,100},
                        centerText = true,
                        layer = 2
                    },
                    Rectangle = {
                        layer = 1,
                        size = {220,120},
                        color = 0xFFFFFFFF
                    },
                    Button = {
                        size = {220,120},
                        params = {3}
                    }
                }
            },
            camera = {
                handler = "Camera",
                components = {
                    Camera = {
                        size = {1920,1080}
                    },
                    Transform = {0}
                }
            }
        },
        components = {
            MovementManager = {
                tileWidth = 200,
                tileHeight =200
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
            --PlayerInput = {0},
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
                            ModifyVariable:new("counter", 2, false),
                            Animation:new({action = "change", animation = "assets/sprites/test2.animation.lua"}),
                            Dialogue:new("¡Hola! Bienvenido al mundo de los Pokémon. Mi nombre es Oak. Pero las personas me llaman Profesor Oak. Este es un mundo vasto y fascinante, lleno de criaturas llamadas Pokémon. Los Pokémon son seres extraordinarios que conviven con los seres humanos, y juntos comparten un sinfín de aventuras y desafíos. En este mundo, los Pokémon pueden ser compañeros leales, amigos y aliados, pero también pueden ser poderosos rivales en emocionantes batallas. Es un mundo donde cada Pokémon tiene su propia historia, habilidades únicas y, lo más importante, una relación especial con las personas que los cuidan.\nLos Pokémon no son solo criaturas para luchar. Son compañeros de vida que ayudan en tareas diarias, en la exploración y, en algunos casos, incluso en la protección de las personas. Sin embargo, no todos los Pokémon llevan una vida tranquila. Algunos de ellos están destinados a enfrentarse en épicas batallas para demostrar quién es el más fuerte. Es en estas batallas donde los entrenadores como tú entran en acción. El destino de los Pokémon y los entrenadores está entrelazado, y juntos, pueden alcanzar nuevas alturas.\nEste es el comienzo de tu propia aventura en el mundo Pokémon. Como futuro entrenador, tu misión será recorrer el mundo, capturar Pokémon, entrenarlos y, lo más importante, descubrir qué significa ser un verdadero entrenador. Pero antes de que comiences tu viaje, hay algo muy importante que debes hacer: elegir a tu primer compañero Pokémon. Elige sabiamente, porque este será tu compañero más cercano, y juntos, enfrentarán muchos desafíos. No solo te acompañará en tus batallas, sino que también será tu amigo, tu apoyo y tu guía mientras exploras este vasto mundo.\nCada elección es importante, y no solo por la fuerza que tu Pokémon pueda tener. Los Pokémon tienen características que se ajustan a diferentes estilos de entrenamiento, y tú tendrás que aprender a conocerlos, a cuidarlos y a fortalecerlos. Así que, sin más preámbulo, elige a tu primer compañero Pokémon y prepárate para comenzar una aventura inolvidable. ¡El mundo Pokémon te espera!"),
                            WaitFor:new({type = "BehaviourEnded", params = { behaviour = 3 }}),
                            Choices:new("counter", {"Sí",false,3}, {"Sí","No","¿Huh?"})
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