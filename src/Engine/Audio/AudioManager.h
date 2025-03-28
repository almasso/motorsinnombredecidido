#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <cstdint>
#include <string>
#include <unordered_map>

typedef uint32_t AudioDevice;
class AudioMixer;
class AudioClip;
struct AudioMixerData;

class AudioClipData;

/// @~english
/// @brief Class in charge of managing the audio system.
/// @~spanish
/// @brief Clase que se encarga de gestionar el sistema de audio.
class AudioManager {
private:
    static AudioManager* _instance;

    AudioDevice _audioDeviceId;
    std::unordered_map<std::string, AudioMixer*> _mixers;


    /// @~english
    /// @brief Creates an empty \c AudioManager .
    /// @~spanish
    /// @brief Crea un \c AudioManager vacío.
    explicit AudioManager();

    /// @~english
    /// @brief Initializes the audio system.
    /// @return \c true on successful initialization. \c false otherwise.
    /// @~spanish
    /// @brief Inicializa el sistema de audio.
    /// @return \c true en inicialización exitosa. \c false si no.
    bool init();

    /// @~english
    /// @brief Creates an instance of an \c AudioMixer and connects it to the system using the provided data.
    /// @remark This method won't create a new \c AudioMixer if there was another with the same name.
    /// @param data Pointer to the resource containing the necessary data to create the \c AudioMixer .
    /// @return Pointer to the created \c AudioMixer . If there was an already registered mixer with the name provided, that instance will be returned instead of a new one.
    /// @~spanish
    /// @brief Crea una instancia de un \c AudioMixer y lo conecta al sistema usando los datos proporcionados.
    /// @remark Este método no creará un nuevo \c AudioMixer si ya había otro con el mismo nombre.
    /// @param data Puntero al recurso conteniendo los datos necesarios para crear el \c AudioMixer .
    /// @return Puntero al \c AudioMixer creado. Si ya había un mezclador registrado con el mismo nombre proporcionado, se devolverá la instancia a ese en lugar de a uno nuevo.
    AudioMixer* registerAudioMixer(AudioMixerData const* data);

public:
    /// @~english
    /// @brief Copy possibility deletion for singleton pattern assurance
    /// @~spanish
    /// @brief Eliminación de la posibilidad de copia para asegurar el patrón \a singleton.
    AudioManager(AudioManager const&) = delete;

    /// @~english
    /// @brief Copy possibility deletion for singleton pattern assurance
    /// @~spanish
    /// @brief Eliminación de la posibilidad de copia para asegurar el patrón \a singleton.
    void operator=(AudioManager const&) = delete;

    /// @~english
    /// @brief Initializes the audio system.
    /// @return \c true on successful initialization. \c false otherwise.
    /// @~spanish
    /// @brief Inicializa el sistema de audio.
    /// @return \c true en inicialización exitosa. \c false si no.
    static bool Init();

    /// @~english
    /// @brief Access to the single instance of \c AudioManager .
    /// @return Pointer to the single instance of \c AudioManager .
    /// @~spanish
    /// @brief Acceso a la instancia única de \c AudioManager .
    /// @return Puntero a la instancia única de \c AudioManager .
    static AudioManager* Instance();

    /// @~english
    /// @brief Stops the audio system.
    /// @~spanish
    /// @brief Detiene el sistema de audio.
    static void Shutdown();

    /// @~english
    /// @brief Stops the audio system.
    /// @~spanish
    /// @brief Detiene el sistema de audio.
    ~AudioManager();

    /// @~english
    /// @brief Access to an \c AudioMixer connected to the audio system.
    /// @param mixer Path to the asset representing the wanted mixer.
    /// @return Pointer to the \c AudioMixer requested. \c nullptr if that mixer does not exist.
    /// @~spanish
    /// @brief Acceso a un \c AudioMixer conectado al sistema de audio.
    /// @param mixer Ruta al recurso representando el mezclador querido.
    /// @return Puntero al \c AudioMixer solicitado. \c nullptr si el mezclador no existe.
    AudioMixer* getMixer(std::string const& mixer);
};

#endif //AUDIOMANAGER_H