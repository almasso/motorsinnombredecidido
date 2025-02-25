#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H
#include <cstdint>

typedef uint32_t AudioDevice;
typedef struct SDL_AudioStream AudioStream;
struct AudioClipData;
class AudioMixer;

/// @~english
/// @brief Data structure used to manage a single instance of an audio clip
/// @~spanish
/// @brief Estructura de datos usada para gestional una única instancia de una pista de audio
class AudioClip {
private:
    enum State {
        PLAYING,
        PAUSED,
        STOPPED
    } state_;
    AudioClipData const* data_;
    AudioStream* stream_;
    AudioMixer* mixer_;
    AudioDevice device_;
    float localVolume_, volume_;
    bool loop_;

    /// @~english
    /// @brief Static method used as a callback to be called every time data is requested from an \c AudioStream.
    /// It is used to control when the clip ended and set it's state to stopped or play it again if it has the loop state active.
    /// @param userdata An opaque pointer containing the \c AudioClip containing the associated \c AudioStream.
    /// @param stream The \c AudioStream associated with this callback.
    /// @param additional_amount The amount of additional data, in bytes, that is needed to satisfy the immediate request.
    /// @param total_amount The total amount of data requested, in bytes, that is being requested or available.
    /// @~spanish
    /// @brief Método estático usado como \a callback a llamar cada vez que información se solicita de un \c AudioStream.
    /// Se usa para controlar cuándo la pista terminó y asignar su estado a detenido o reproducirlo otra vez si tiene el estado de ciclado activo.
    /// @param userdata Un puntero opaco que contiene el \c AudioClip que contiene el \c AudioStream asociado.
    /// @param stream El \c AudioStream asociado con este \a callback.
    /// @param additional_amount La cantidad de información adicional, en bytes, que se necesita para satisfacer la solicitud inmediata.
    /// @param total_amount La cantidad total de información solicitada, en bytes, que se está solicitando o está disponible.
    static void Update(void* userdata, AudioStream* stream, int additional_amount, int total_amount);

    /// @~english
    /// @brief Stops the clip and unassigns the \c AudioMixer and the \c AudioDevice.
    /// @~spanish
    /// @brief Detiene reproducción de la pista y desasigna el \c AudioMixer y el \c AudioDevice.
    void reset();
    
public:
    /// @~english
    /// @brief Creates a new clip with de data provided.
    /// @remark The clip is created stopped, with no \c AudioMixer or \c AudioDevice, with its volume set to \c 1.0f and without loop state.
    /// @~spanish
    /// @brief Crea una nueva pista con la información dada.
    /// @remark La pista se crea detenida, sin \c AudioMixer o \c AudioDevice, con el volumen asignado a \c 1.0f y sin el estado de ciclado.
    explicit AudioClip(AudioClipData const* data);

    /// @~english
    /// @brief Stops the clip and unassigns the \c AudioMixer and the \c AudioDevice.
    /// @~spanish
    /// @brief Detiene reproducción de la pista y desasigna el \c AudioMixer y el \c AudioDevice.
    ~AudioClip();

    /// @~english
    /// @brief Plays the clip from the start.
    /// @remark If the clip was already playing or paused it will be restarted.
    /// @return \c true if the clip played correctly; \c false otherwise, programmer must make sure that an \c AudioDevice or an \c AudioMixer have been assigned to the clip.
    /// @~spanish
    /// @brief Reproduce la pista desde el principio.
    /// @remark Si la pista ya se estaba reproduciendo o estaba pausada se reiniciará.
    /// @return \c true si la pista se reprodujo correctamente; \c false si no, el programador debe asegurarse de que la pista tiene un \c AudioDevice o un \c AudioMixer asignado.
    bool play();

    /// @~english
    /// @brief Stops the clip completely
    /// @return \c true if the clip stopped correctly; \c false otherwise, programmer must make sure that an \c AudioDevice or an \c AudioMixer have been assigned to the clip.
    /// @~spanish
    /// @brief Detiene la pista por completo
    /// @return \c true si la pista se detuvo correctamente; \c false si no, el programador debe asegurarse de que la pista tiene un \c AudioDevice o un \c AudioMixer asignado.
    bool stop();

    /// @~english
    /// @brief Pauses the clip
    /// @~spanish
    /// @brief Pausa la pista
    void pause();

    /// @~english
    /// @brief Resumes the clip at the point it was paused
    /// @return \c true if the clip resumed correctly or if it was already playing; \c false otherwise, programmer must make sure that an \c AudioDevice or an \c AudioMixer have been assigned to the clip.
    bool resume();

    /// @~english
    /// @brief Checks whether the clip is currently playing
    /// @return \c true if the clip is currently playing, \c false otherwise
    /// @~spanish
    /// @brief Comprueba si la pista se está reproduciendo actualmente
    /// @return \c true si la pista se está reproduciendo actualmente, \c false si no
    bool isPlaying() const;

    /// @~english
    /// @brief Checks whether the clip is currently paused
    /// @return \c true if the clip is currently paused, \c false otherwise
    /// @~spanish
    /// @brief Comprueba si la pista está en pausa actualmente
    /// @return \c true si la pista está en pausa actualmente, \c false si no
    bool isPaused() const;

    /// @~english
    /// @brief Gets the clip's current volume
    /// @return Value between \c 0.0f and \c 1.0f representing the volume assigned to the clip
    /// @~spanish
    /// @brief Accede al volumen actual de la pista
    /// @return Valor entre \c 0.0f y \c 1.0f representando el volumen asignado a la pista
    float getVolume() const;

    /// @~english
    /// @brief Sets the clip's current volume
    /// @param volume Value between \c 0.0f and \c 1.0f representing the volume to be assigned to the clip
    /// @~spanish
    /// @brief Establece el volumen actual de la pista
    /// @param volume Valor entre \c 0.0f y \c 1.0f representando el volumen a asignar a la pista
    void setVolume(float volume);

    /// @~english
    /// @brief Checks whether the clip has the loop option active
    /// @return \c true if the loop is active, \c false otherwise
    /// @~spanish
    /// @brief Comprueba si la pista tiene la opción de ciclado activa
    /// @return \c true si el ciclado está activado, \c false si no
    bool isLooped() const;

    /// @~english
    /// @brief Sets the current loop state of the clip
    /// @param loop \c true to activate the loop state, \c false to deactivate it
    /// @~spanish
    /// @brief Establece el estado de ciclado de la pista
    /// @param loop \c true para activar el estado de ciclado, \c false para desactivarlo
    void setLoop(bool loop);

    /// @~english
    /// @brief Updates the clip's global playing volume depending on the clip's volume and on it's \c AudioMixer 's if it has any assigned.
    /// @remark This method is called automatically every time the clip's volume changes or every time it's \c AudioMixer 's global volume if it has any assigned. Programmer doesn't need to call this method at any given point.
    /// @~spanish
    /// @brief Actualiza el volumen global de reproducción de la pista dependiendo del volumen de la pista y su \c AudioMixer si tiene alguno asignado.
    /// @remark Este método se llama automáticamente cada vez que se cambia el volumen de la pista o el volumen global de su \c AudioMixer si tiene alguno asignado. El programador no tiene la necesidad de llamar a este método en ningun momento.
    void updateVolume();

    /// @~english
    /// @brief Assigns an \c AudioMixer to this clip.
    /// @remark Every change made to the \c AudioMixer will now affect this clip.
    /// @param mixer \c AudioMixer to assign to the clip.
    /// @~spanish
    /// @brief Asigna un \c AudioMixer a esta pista.
    /// @remark Cada cambio hecho al \c AudioMixer afectará a partir de ahora a esta pista.
    /// @param mixer \c AudioMixer a asignar a la pista.
    void assignMixer(AudioMixer* mixer);

    /// @~english
    /// @brief Assigns an \c AudioDevice through which this clip will play.
    /// @remark If an \c AudioMixer was assigned to this clip, that \c AudioMixer 's assigned \c AudioDevice will also be assigned to this clip. Programmer cannot override a clip's \c AudioDevice if that clip is connected to an \c AudioMixer ; to do that the clip's \c AudioMixer should be assigned to \c nullptr .
    /// @param device \c AudioDevice where the clip is desired to play.
    /// @~spanish
    /// @brief Asigna un \c AudioDevice a través del cual esta pista se reproducirá.
    /// @remark Si esta pista tiene un \c AudioMixer asignado, el \c AudioDevice asignado a ese \c AudioMixer será también asignado a esta pista. El programador no puede sobreescribir el \c AudioDevice de una pista si esa pista está conectada a un \c AudioMixer ; para hacerlo el \c AudioMixer de la pista debería estar asignado a \c nullptr .
    /// @param device \c AudioDevice donde se desea que se reproduzca la pista.
    void assignDevice(AudioDevice device);
};


#endif //AUDIOCLIP_H
