#ifndef AUDIOMIXER_H
#define AUDIOMIXER_H
#include <cstdint>
#include <unordered_set>

class AudioClip;
typedef uint32_t AudioDevice;

/// @~english
/// @brief Class made to englobe changes wanted to be applied to multiple \c AudioClip s. \c AudioMixer instances may be connected to each other to chain its effects.
/// @~spanish
/// @brief Clase creada para englobar cambios que se quieren aplicar a múltiples \c AudioClip s. Las instancias de \c AudioMixer pueden conectarse entre sí para encadenar sus efectos.
class AudioMixer {
private:
    AudioDevice audioDevice_;
    AudioMixer* output_;
    std::unordered_set<AudioMixer*> mixers_;
    std::unordered_set<AudioClip*> clips_;

    float localVolume_, volume_;
public:
    /// @~english
    /// @brief Creates a new mixer.
    /// @remark The mixer created will have no \c AudioDevice assigned, the volume set to \1.0f .
    /// @~spanish
    /// @brief Crea un nuevo mezclador.
    /// @remark El mezclador creado no tendrá asignado ningún \c AudioDevice y el volumen estará asignado a \c 1.0f .
    AudioMixer();

    /// @~english
    /// @brief Connects an \c AudioClip to this mixer. Every change made to this mixer will now affect the connected \c AudioClip.
    /// @param clip \c AudioClip to be connected to this mixer.
    /// @~spanish
    /// @brief Conecta un \c AudioClip a este mezclador. Todos los cambios hechos a este mezclador afectarán al \c AudioClip conectado.
    /// @param clip \c AudioClip a conectar a este mezclador.
    void connect(AudioClip* clip);

    /// @~english
    /// @brief Disconnects an \c AudioClip from this mixer. All changes from this mixer applied to this \c AudioClip will be undone.
    /// @param clip \c AudioClip to be disconnected from this mixer.
    /// @~spanish
    /// @brief Desconecta un \c AudioClip de este mezclador. Todos los cambios de este mezclador aplicados a este \c AudioClip se desharán.
    /// @param clip \c AudioClip a desconectar de este mezclador.
    void disconnect(AudioClip* clip);

    /// @~english
    /// @brief Connects another \c AudioMixer to this mixer. Every change made to this mixer will now affect the connected \c AudioMixer.
    /// @param mixer \c AudioMixer to be connected to this mixer.
    /// @~spanish
    /// @brief Conecta otro \c AudioMixer a este mezclador. Todos los cambios hechos a este mezclador afectarán al \c AudioMixer conectado.
    /// @param mixer \c AudioMixer a conectar a este mezclador.
    void connect(AudioMixer* mixer);

    /// @~english
    /// @brief Disconnects an \c AudioMixer from this mixer. All changes from this mixer applied to that \c AudioMixer will be undone.
    /// @param mixer \c AudioMixer to be disconnected from this mixer.
    /// @~spanish
    /// @brief Desconecta un \c AudioMixer de este mezclador. Todos los cambios de este mezclador aplicados a ese \c AudioMixer se desharán.
    /// @param mixer \c AudioMixer a desconectar de este mezclador.
    void disconnect(AudioMixer* mixer);

    /// @~english
    /// @brief Sets the mixer's current volume.
    /// @param volume Value between \c 0.0f and \c 1.0f representing the volume assigned to the mixer.
    /// @~spanish
    /// @brief Establece el volumen actual del mezclador.
    /// @param volume Valor entre \c 0.0f y \c 1.0f representando el volumen asignado al mezclador.
    void setVolume(float volume);

    /// @~english
    /// @brief Gets the local volume assigned to the mixer.
    /// @return Value between \c 0.0f and \c 1.0f representing the volume assigned to the mixer.
    /// @~spanish
    /// @brief Accede al volumen local asignado al mezclador.
    /// @return Valor entre \c 0.0f y \c 1.0f representando el volumen local asignado al mezclador.
    float getVolume() const;

    /// @~english
    /// @brief Gets the volume the mixer outputs.
    /// @return Value between \c 0.0f and \c 1.0f representing the volume the mixer outputs.
    /// @~spanish
    /// @brief Accede al volumen al que el audio sale del mezclador.
    /// @return Valor entre \c 0.0f y \c 1.0f representando el volumen al que el audio sale del mezclador.
    float getGlobalVolume() const;

    /// @~english
    /// @brief Updates the mixer's global playing volume depending on the mixer's volume and on it's output \c AudioMixer 's if it has any assigned.
    /// @remark This method is called automatically every time the mixer's volume changes or every time it's output \c AudioMixer 's global volume changes if it has any assigned. Programmer doesn't need to call this method at any given point.
    /// @~spanish
    /// @brief Actualiza el volumen global de reproducción del mezclador dependiendo del volumen del mezclador y su \c AudioMixer de salida si tiene alguno asignado.
    /// @remark Este método se llama automáticamente cada vez que se cambia el volumen del mezclador o el volumen global de su \c AudioMixer de salida si tiene alguno asignado. El programador no tiene la necesidad de llamar a este método en ningun momento.
    void updateVolume();

    /// @~english
    /// @brief Assigns an \c AudioDevice through which this mixer's connected clips will play.
    /// @remark If this mixer was connected to another \c AudioMixer, that \c AudioMixer 's assigned \c AudioDevice will also be assigned to this mixer. Programmer cannot override a mixer's \c AudioDevice if that mixer is connected to another \c AudioMixer ; to do that the mixer should be disconnected from it's output \c AudioMixer .
    /// @param device \c AudioDevice where the clip is desired to play.
    /// @~spanish
    /// @brief Asigna un \c AudioDevice a través del cual este mezclador se reproducirá.
    /// @remark Si este mezclador está conectado a otro \c AudioMixer, el \c AudioDevice asignado a ese \c AudioMixer será también asignado a este mezclador. El programador no puede sobreescribir el \c AudioDevice de un mezclador si ese mezclador está conectado a otro \c AudioMixer ; para hacerlo el mezclador debería desconectarse de su \c AudioMixer de salida.
    /// @param device \c AudioDevice donde se desea que se reproduzca la pista.
    void assignDevice(AudioDevice device);

    /// @~english
    /// @brief Gets the \c AudioDevice previously assigned to this mixer.
    /// @return The \c AudioDevice assigned to this mixer. \c 0 if there wasn't any.
    /// @~spanish
    /// @brief Accede al \c AudioDevice previamente asignado a este mezclador.
    /// @return El \c AudioDevice asignado a este mezclador. \c 0 si no se asignó ninguno.
    AudioDevice getDevice() const;
};


#endif //AUDIOMIXER_H
