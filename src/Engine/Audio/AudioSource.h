#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <sol/forward.hpp>
#include <Core/ComponentTemplate.h>

class AudioClip;
class AudioMixer;

/// @~english
/// @brief Component containing a source of audio and allows to play it.
/// @~spanish
/// @brief Componente que contienen una fuente de audio y permite reproducirla.
class AudioSource : public ComponentTemplate<AudioSource, "AudioSource"> {
private:
    AudioClip* _clip;
    AudioMixer* _mixer;
    
public:
    explicit AudioSource(ComponentData const* data);
    ~AudioSource() override;
    bool init() override;
    void onEnable() override;
    void onDisable() override;

    /// @~english
    /// @brief Plays the \c AudioSource from the start.
    /// @remark If the source was already playing or paused it will be restarted.
    /// @return \c true if the source played correctly; \c false otherwise.
    /// @~spanish
    /// @brief Reproduce la \c AudioSource desde el principio.
    /// @remark Si la fuente ya se estaba reproduciendo o estaba pausada se reiniciará.
    /// @return \c true si la fuente se reprodujo correctamente; \c false si no.
    void play();

    /// @~english
    /// @brief Stops the \c AudioSource completely.
    /// @return \c true if the source stopped correctly; \c false otherwise.
    /// @~spanish
    /// @brief Detiene la \c AudioSource por completo.
    /// @return \c true si la fuente se detuvo correctamente; \c false si no.
    void stop();

    /// @~english
    /// @brief Pauses the \c AudioSource.
    /// @~spanish
    /// @brief Pausa la pista.
    void pause();

    /// @~english
    /// @brief Resumes the \c AudioSource at the point it was paused.
    /// @return \c true if the source resumed correctly or if it was already playing; \c false otherwise.
    /// @~spanish
    /// @brief Reproduce la \c AudioSource desde el punto en el que se pausó.
    /// @return \c true si la fuente se reprodujo correctamente o si ya se estaba reproduciendo; \c false si no.
    void resume();

    /// @~english
    /// @brief Checks whether the \c AudioSource is currently playing.
    /// @return \c true if the \c AudioSource is currently playing, \c false otherwise.
    /// @~spanish
    /// @brief Comprueba si la \c AudioSource se está reproduciendo actualmente.
    /// @return \c true si la \c AudioSource se está reproduciendo actualmente, \c false si no.
    bool isPlaying() const;

    /// @~english
    /// @brief Checks whether the \c AudioSource is currently paused.
    /// @return \c true if the \c AudioSource is currently paused, \c false otherwise.
    /// @~spanish
    /// @brief Comprueba si la \c AudioSource está en pausa actualmente.
    /// @return \c true si la \c AudioSource está en pausa actualmente, \c false si no.
    bool isPaused() const;

    /// @~english
    /// @brief Gets the \c AudioSource 's current volume.
    /// @return Value between \c 0.0f and \c 1.0f representing the volume assigned to the source.
    /// @~spanish
    /// @brief Accede al volumen actual de la \c AudioSource .
    /// @return Valor entre \c 0.0f y \c 1.0f representando el volumen asignado a la fuente.
    float getVolume() const;

    /// @~english
    /// @brief Sets the \c AudioSource 's current volume.
    /// @param volume Value between \c 0.0f and \c 1.0f representing the volume to be assigned to the source.
    /// @~spanish
    /// @brief Establece el volumen actual de la \c AudioSource .
    /// @param volume Valor entre \c 0.0f y \c 1.0f representando el volumen a asignar a la fuente.
    void setVolume(float volume);

    /// @~english
    /// @brief Checks whether the \c AudioSource has the loop option active.
    /// @return \c true if the loop is active, \c false otherwise.
    /// @~spanish
    /// @brief Comprueba si la \c AudioSource tiene la opción de ciclado activa.
    /// @return \c true si el ciclado está activado, \c false si no.
    bool isLooped() const;

    /// @~english
    /// @brief Sets the current loop state of the \c AudioSource .
    /// @param loop \c true to activate the loop state, \c false to deactivate it.
    /// @~spanish
    /// @brief Establece el estado de ciclado de la \c AudioSource .
    /// @param loop \c true para activar el estado de ciclado, \c false para desactivarlo.
    void setLoop(bool loop);

    static void RegisterToLua(sol::state& luaState);
};



#endif //AUDIOSOURCE_H
