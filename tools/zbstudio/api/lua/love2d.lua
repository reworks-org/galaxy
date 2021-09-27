-- Copyright 2011-18 Paul Kulchenko, ZeroBrane LLC

-- Converted from love_api.lua in https://github.com/love2d-community/love-api
-- (API for LÖVE 11.3 as of Dec 12, 2019)
-- The conversion script is at the bottom of this file

-- To process:
-- 1. clone love-api and copy love_api.lua and modules/ folder to ZBS/api/lua folder
-- 2. run "../../bin/lua love2d.lua >newapi" from ZBS/api/lua folder
-- 3. copy the content of "newapi" file to replace "love" table in love2d.lua
--    (make sure that the generated content is copied as UTF-8/65001 encoding)
-- 4. launch the IDE and switch to love2d to confirm that it's loading without issues

local love = {
 childs = {
  ByteData = {
   description = "Data object containing arbitrary bytes in an contiguous memory.\n\nThere are currently no LÖVE functions provided for manipulating the contents of a ByteData, but Data:getPointer can be used with LuaJIT's FFI to access and write to the contents directly.",
   inherits = "love.Object, love.Data",
   type = "class"
  },
  Data = {
   description = "The superclass of all data.",
   inherits = "love.Object",
   type = "class"
  },
  Drawable = {
   description = "Superclass for all things that can be drawn on screen. This is an abstract type that can't be created directly.",
   inherits = "love.Object",
   type = "class"
  },
  Object = {
   description = "The superclass of all LÖVE types.",
   type = "class"
  },
  audio = {
   childs = {
    DistanceModel = {
     childs = {
      exponent = {
       description = "Exponential attenuation.",
       type = "value"
      },
      exponentclamped = {
       description = "Exponential attenuation. Gain is clamped. In version 0.9.2 and older this is named '''exponent clamped'''.",
       type = "value"
      },
      inverse = {
       description = "Inverse distance attenuation.",
       type = "value"
      },
      inverseclamped = {
       description = "Inverse distance attenuation. Gain is clamped. In version 0.9.2 and older this is named '''inverse clamped'''.",
       type = "value"
      },
      linear = {
       description = "Linear attenuation.",
       type = "value"
      },
      linearclamped = {
       description = "Linear attenuation. Gain is clamped. In version 0.9.2 and older this is named '''linear clamped'''.",
       type = "value"
      },
      none = {
       description = "Sources do not get attenuated.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    EffectType = {
     childs = {
      chorus = {
       description = "Plays multiple copies of the sound with slight pitch and time variation. Used to make sounds sound \"fuller\" or \"thicker\".",
       type = "value"
      },
      compressor = {
       description = "Decreases the dynamic range of the sound, making the loud and quiet parts closer in volume, producing a more uniform amplitude throughout time.",
       type = "value"
      },
      distortion = {
       description = "Alters the sound by amplifying it until it clips, shearing off parts of the signal, leading to a compressed and distorted sound.",
       type = "value"
      },
      echo = {
       description = "Decaying feedback based effect, on the order of seconds. Also known as delay; causes the sound to repeat at regular intervals at a decreasing volume.",
       type = "value"
      },
      equalizer = {
       description = "Adjust the frequency components of the sound using a 4-band (low-shelf, two band-pass and a high-shelf) equalizer.",
       type = "value"
      },
      flanger = {
       description = "Plays two copies of the sound; while varying the phase, or equivalently delaying one of them, by amounts on the order of milliseconds, resulting in phasing sounds.",
       type = "value"
      },
      reverb = {
       description = "Decaying feedback based effect, on the order of milliseconds. Used to simulate the reflection off of the surroundings.",
       type = "value"
      },
      ringmodulator = {
       description = "An implementation of amplitude modulation; multiplies the source signal with a simple waveform, to produce either volume changes, or inharmonic overtones.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    EffectWaveform = {
     childs = {
      sawtooth = {
       description = "A sawtooth wave, also known as a ramp wave. Named for its linear rise, and (near-)instantaneous fall along time.",
       type = "value"
      },
      sine = {
       description = "A sine wave. Follows a trigonometric sine function.",
       type = "value"
      },
      square = {
       description = "A square wave. Switches between high and low states (near-)instantaneously.",
       type = "value"
      },
      triangle = {
       description = "A triangle wave. Follows a linear rise and fall that repeats periodically.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    FilterType = {
     childs = {
      bandpass = {
       description = "Band-pass filter. Both high and low frequency sounds are attenuated based on the given parameters.",
       type = "value"
      },
      highpass = {
       description = "High-pass filter. Low frequency sounds are attenuated.",
       type = "value"
      },
      lowpass = {
       description = "Low-pass filter. High frequency sounds are attenuated.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    RecordingDevice = {
     childs = {
      getBitDepth = {
       args = "()",
       description = "Gets the number of bits per sample in the data currently being recorded.",
       returns = "(bits: number)",
       type = "function"
      },
      getChannelCount = {
       args = "()",
       description = "Gets the number of channels currently being recorded (mono or stereo).",
       returns = "(channels: number)",
       type = "function"
      },
      getData = {
       args = "()",
       description = "Gets all recorded audio SoundData stored in the device's internal ring buffer.\n\nThe internal ring buffer is cleared when this function is called, so calling it again will only get audio recorded after the previous call. If the device's internal ring buffer completely fills up before getData is called, the oldest data that doesn't fit into the buffer will be lost.",
       returns = "(data: SoundData)",
       type = "function",
       valuetype = "love.sound.SoundData"
      },
      getName = {
       args = "()",
       description = "Gets the name of the recording device.",
       returns = "(name: string)",
       type = "function"
      },
      getSampleCount = {
       args = "()",
       description = "Gets the number of currently recorded samples.",
       returns = "(samples: number)",
       type = "function"
      },
      getSampleRate = {
       args = "()",
       description = "Gets the number of samples per second currently being recorded.",
       returns = "(rate: number)",
       type = "function"
      },
      isRecording = {
       args = "()",
       description = "Gets whether the device is currently recording.",
       returns = "(recording: boolean)",
       type = "function"
      },
      start = {
       args = "(samplecount: number, samplerate: number, bitdepth: number, channels: number)",
       description = "Begins recording audio using this device.",
       returns = "(success: boolean)",
       type = "function"
      },
      stop = {
       args = "()",
       description = "Stops recording audio from this device. Any sound data currently in the device's buffer will be returned.",
       returns = "(data: SoundData)",
       type = "function",
       valuetype = "love.sound.SoundData"
      }
     },
     description = "Represents an audio input device capable of recording sounds.",
     inherits = "love.Object",
     type = "class"
    },
    Source = {
     childs = {
      getActiveEffects = {
       args = "()",
       description = "Gets a list of the Source's active effect names.",
       returns = "(effects: table)",
       type = "function"
      },
      getAirAbsorption = {
       args = "()",
       description = "Gets the amount of air absorption applied to the Source.\n\nBy default the value is set to 0 which means that air absorption effects are disabled. A value of 1 will apply high frequency attenuation to the Source at a rate of 0.05 dB per meter.",
       returns = "(amount: number)",
       type = "function"
      },
      getAttenuationDistances = {
       args = "()",
       description = "Gets the reference and maximum attenuation distances of the Source. The values, combined with the current DistanceModel, affect how the Source's volume attenuates based on distance from the listener.",
       returns = "(ref: number, max: number)",
       type = "function"
      },
      getChannelCount = {
       args = "()",
       description = "Gets the number of channels in the Source. Only 1-channel (mono) Sources can use directional and positional effects.",
       returns = "(channels: number)",
       type = "function"
      },
      getCone = {
       args = "()",
       description = "Gets the Source's directional volume cones. Together with Source:setDirection, the cone angles allow for the Source's volume to vary depending on its direction.",
       returns = "(innerAngle: number, outerAngle: number, outerVolume: number)",
       type = "function"
      },
      getDirection = {
       args = "()",
       description = "Gets the direction of the Source.",
       returns = "(x: number, y: number, z: number)",
       type = "function"
      },
      getDuration = {
       args = "(unit: TimeUnit)",
       description = "Gets the duration of the Source. For streaming Sources it may not always be sample-accurate, and may return -1 if the duration cannot be determined at all.",
       returns = "(duration: number)",
       type = "function"
      },
      getEffect = {
       args = "(name: string, filtersettings: table)",
       description = "Gets the filter settings associated to a specific effect.\n\nThis function returns nil if the effect was applied with no filter settings associated to it.",
       returns = "(filtersettings: table)",
       type = "function"
      },
      getFilter = {
       args = "()",
       description = "Gets the filter settings currently applied to the Source.",
       returns = "(settings: table)",
       type = "function"
      },
      getFreeBufferCount = {
       args = "()",
       description = "Gets the number of free buffer slots in a queueable Source. If the queueable Source is playing, this value will increase up to the amount the Source was created with. If the queueable Source is stopped, it will process all of its internal buffers first, in which case this function will always return the amount it was created with.",
       returns = "(buffers: number)",
       type = "function"
      },
      getPitch = {
       args = "()",
       description = "Gets the current pitch of the Source.",
       returns = "(pitch: number)",
       type = "function"
      },
      getPosition = {
       args = "()",
       description = "Gets the position of the Source.",
       returns = "(x: number, y: number, z: number)",
       type = "function"
      },
      getRolloff = {
       args = "()",
       description = "Returns the rolloff factor of the source.",
       returns = "(rolloff: number)",
       type = "function"
      },
      getType = {
       args = "()",
       description = "Gets the type of the Source.",
       returns = "(sourcetype: SourceType)",
       type = "function"
      },
      getVelocity = {
       args = "()",
       description = "Gets the velocity of the Source.",
       returns = "(x: number, y: number, z: number)",
       type = "function"
      },
      getVolume = {
       args = "()",
       description = "Gets the current volume of the Source.",
       returns = "(volume: number)",
       type = "function"
      },
      getVolumeLimits = {
       args = "()",
       description = "Returns the volume limits of the source.",
       returns = "(min: number, max: number)",
       type = "function"
      },
      isLooping = {
       args = "()",
       description = "Returns whether the Source will loop.",
       returns = "(loop: boolean)",
       type = "function"
      },
      isPlaying = {
       args = "()",
       description = "Returns whether the Source is playing.",
       returns = "(playing: boolean)",
       type = "function"
      },
      isRelative = {
       args = "()",
       description = "Gets whether the Source's position, velocity, direction, and cone angles are relative to the listener.",
       returns = "(relative: boolean)",
       type = "function"
      },
      pause = {
       args = "()",
       description = "Pauses the Source.",
       returns = "()",
       type = "function"
      },
      play = {
       args = "()",
       description = "Starts playing the Source.",
       returns = "(success: boolean)",
       type = "function"
      },
      queue = {
       args = "(sounddata: SoundData)",
       description = "Queues SoundData for playback in a queueable Source.\n\nThis method requires the Source to be created via love.audio.newQueueableSource.",
       returns = "(success: boolean)",
       type = "function"
      },
      seek = {
       args = "(offset: number, unit: TimeUnit)",
       description = "Sets the currently playing position of the Source.",
       returns = "()",
       type = "function"
      },
      setAirAbsorption = {
       args = "(amount: number)",
       description = "Sets the amount of air absorption applied to the Source.\n\nBy default the value is set to 0 which means that air absorption effects are disabled. A value of 1 will apply high frequency attenuation to the Source at a rate of 0.05 dB per meter.\n\nAir absorption can simulate sound transmission through foggy air, dry air, smoky atmosphere, etc. It can be used to simulate different atmospheric conditions within different locations in an area.",
       returns = "()",
       type = "function"
      },
      setAttenuationDistances = {
       args = "(ref: number, max: number)",
       description = "Sets the reference and maximum attenuation distances of the Source. The parameters, combined with the current DistanceModel, affect how the Source's volume attenuates based on distance.\n\nDistance attenuation is only applicable to Sources based on mono (rather than stereo) audio.",
       returns = "()",
       type = "function"
      },
      setCone = {
       args = "(innerAngle: number, outerAngle: number, outerVolume: number)",
       description = "Sets the Source's directional volume cones. Together with Source:setDirection, the cone angles allow for the Source's volume to vary depending on its direction.",
       returns = "()",
       type = "function"
      },
      setDirection = {
       args = "(x: number, y: number, z: number)",
       description = "Sets the direction vector of the Source. A zero vector makes the source non-directional.",
       returns = "()",
       type = "function"
      },
      setEffect = {
       args = "(name: string, enable: boolean)",
       description = "Applies an audio effect to the Source.\n\nThe effect must have been previously defined using love.audio.setEffect.",
       returns = "(success: boolean)",
       type = "function"
      },
      setFilter = {
       args = "(settings: table)",
       description = "Sets a low-pass, high-pass, or band-pass filter to apply when playing the Source.",
       returns = "(success: boolean)",
       type = "function"
      },
      setLooping = {
       args = "(loop: boolean)",
       description = "Sets whether the Source should loop.",
       returns = "()",
       type = "function"
      },
      setPitch = {
       args = "(pitch: number)",
       description = "Sets the pitch of the Source.",
       returns = "()",
       type = "function"
      },
      setPosition = {
       args = "(x: number, y: number, z: number)",
       description = "Sets the position of the Source. Please note that this only works for mono (i.e. non-stereo) sound files!",
       returns = "()",
       type = "function"
      },
      setRelative = {
       args = "(enable: boolean)",
       description = "Sets whether the Source's position, velocity, direction, and cone angles are relative to the listener, or absolute.\n\nBy default, all sources are absolute and therefore relative to the origin of love's coordinate system 0, 0. Only absolute sources are affected by the position of the listener. Please note that positional audio only works for mono (i.e. non-stereo) sources. ",
       returns = "()",
       type = "function"
      },
      setRolloff = {
       args = "(rolloff: number)",
       description = "Sets the rolloff factor which affects the strength of the used distance attenuation.\n\nExtended information and detailed formulas can be found in the chapter '3.4. Attenuation By Distance' of OpenAL 1.1 specification.",
       returns = "()",
       type = "function"
      },
      setVelocity = {
       args = "(x: number, y: number, z: number)",
       description = "Sets the velocity of the Source.\n\nThis does '''not''' change the position of the Source, but lets the application know how it has to calculate the doppler effect.",
       returns = "()",
       type = "function"
      },
      setVolume = {
       args = "(volume: number)",
       description = "Sets the current volume of the Source.",
       returns = "()",
       type = "function"
      },
      setVolumeLimits = {
       args = "(min: number, max: number)",
       description = "Sets the volume limits of the source. The limits have to be numbers from 0 to 1.",
       returns = "()",
       type = "function"
      },
      stop = {
       args = "()",
       description = "Stops a Source.",
       returns = "()",
       type = "function"
      },
      tell = {
       args = "(unit: TimeUnit)",
       description = "Gets the currently playing position of the Source.",
       returns = "(position: number)",
       type = "function"
      }
     },
     description = "A Source represents audio you can play back.\n\nYou can do interesting things with Sources, like set the volume, pitch, and its position relative to the listener. Please note that positional audio only works for mono (i.e. non-stereo) sources.\n\nThe Source controls (play/pause/stop) act according to the following state table.",
     inherits = "love.Object",
     type = "class"
    },
    SourceType = {
     childs = {
      queue = {
       description = "The audio must be manually queued by the user.",
       type = "value"
      },
      static = {
       description = "The whole audio is decoded.",
       type = "value"
      },
      stream = {
       description = "The audio is decoded in chunks when needed.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    TimeUnit = {
     childs = {
      samples = {
       description = "Audio samples.",
       type = "value"
      },
      seconds = {
       description = "Regular seconds.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    getActiveSourceCount = {
     args = "()",
     description = "Gets the current number of simultaneously playing sources.",
     returns = "(count: number)",
     type = "function"
    },
    getDistanceModel = {
     args = "()",
     description = "Returns the distance attenuation model.",
     returns = "(model: DistanceModel)",
     type = "function"
    },
    getDopplerScale = {
     args = "()",
     description = "Gets the current global scale factor for velocity-based doppler effects.",
     returns = "(scale: number)",
     type = "function"
    },
    getEffect = {
     args = "(name: string)",
     description = "Gets the settings associated with an effect.",
     returns = "(settings: table)",
     type = "function"
    },
    getMaxSceneEffects = {
     args = "()",
     description = "Gets the maximum number of active effects supported by the system.",
     returns = "(maximum: number)",
     type = "function"
    },
    getMaxSourceEffects = {
     args = "()",
     description = "Gets the maximum number of active Effects in a single Source object, that the system can support.",
     returns = "(maximum: number)",
     type = "function"
    },
    getOrientation = {
     args = "()",
     description = "Returns the orientation of the listener.",
     returns = "(fx, fy, fz: number, ux, uy, uz: number)",
     type = "function"
    },
    getPosition = {
     args = "()",
     description = "Returns the position of the listener. Please note that positional audio only works for mono (i.e. non-stereo) sources.",
     returns = "(x: number, y: number, z: number)",
     type = "function"
    },
    getRecordingDevices = {
     args = "()",
     description = "Gets a list of RecordingDevices on the system.\n\nThe first device in the list is the user's default recording device. The list may be empty if there are no microphones connected to the system.\n\nAudio recording is currently not supported on iOS.",
     returns = "(devices: table)",
     type = "function"
    },
    getSourceCount = {
     args = "()",
     description = "Gets the current number of simultaneously playing sources.",
     returns = "(numSources: number)",
     type = "function"
    },
    getVelocity = {
     args = "()",
     description = "Returns the velocity of the listener.",
     returns = "(x: number, y: number, z: number)",
     type = "function"
    },
    getVolume = {
     args = "()",
     description = "Returns the master volume.",
     returns = "(volume: number)",
     type = "function"
    },
    isEffectsSupported = {
     args = "()",
     description = "Gets whether audio effects are supported in the system.",
     returns = "(supported: boolean)",
     type = "function"
    },
    newQueueableSource = {
     args = "(samplerate: number, bitdepth: number, channels: number, buffercount: number)",
     description = "Creates a new Source usable for real-time generated sound playback with Source:queue.",
     returns = "(source: Source)",
     type = "function",
     valuetype = "love.audio.Source"
    },
    newSource = {
     args = "(filename: string, type: SourceType)",
     description = "Creates a new Source from a filepath, File, Decoder or SoundData.\n\nSources created from SoundData are always static.",
     returns = "(source: Source)",
     type = "function",
     valuetype = "love.audio.Source"
    },
    pause = {
     args = "(source: Source, ...: Source)",
     description = "Pauses specific or all currently played Sources.",
     returns = "(Sources: table)",
     type = "function"
    },
    play = {
     args = "(source: Source)",
     description = "Plays the specified Source.",
     returns = "()",
     type = "function"
    },
    setDistanceModel = {
     args = "(model: DistanceModel)",
     description = "Sets the distance attenuation model.",
     returns = "()",
     type = "function"
    },
    setDopplerScale = {
     args = "(scale: number)",
     description = "Sets a global scale factor for velocity-based doppler effects. The default scale value is 1.",
     returns = "()",
     type = "function"
    },
    setEffect = {
     args = "(name: string, settings: table)",
     description = "Defines an effect that can be applied to a Source.\n\nNot all system supports audio effects. Use love.audio.isEffectsSupported to check.",
     returns = "(success: boolean)",
     type = "function"
    },
    setMixWithSystem = {
     args = "(mix: boolean)",
     description = "Sets whether the system should mix the audio with the system's audio.",
     returns = "(success: boolean)",
     type = "function"
    },
    setOrientation = {
     args = "(fx, fy, fz: number, ux, uy, uz: number)",
     description = "Sets the orientation of the listener.",
     returns = "()",
     type = "function"
    },
    setPosition = {
     args = "(x: number, y: number, z: number)",
     description = "Sets the position of the listener, which determines how sounds play.",
     returns = "()",
     type = "function"
    },
    setVelocity = {
     args = "(x: number, y: number, z: number)",
     description = "Sets the velocity of the listener.",
     returns = "()",
     type = "function"
    },
    setVolume = {
     args = "(volume: number)",
     description = "Sets the master volume.",
     returns = "()",
     type = "function"
    },
    stop = {
     args = "(source: Source)",
     description = "Stops currently played sources.",
     returns = "()",
     type = "function"
    }
   },
   description = "Provides an interface to create noise with the user's speakers.",
   type = "class"
  },
  conf = {
   args = "(t: table)",
   description = "If a file called conf.lua is present in your game folder (or .love file), it is run before the LÖVE modules are loaded. You can use this file to overwrite the love.conf function, which is later called by the LÖVE 'boot' script. Using the love.conf function, you can set some configuration options, and change things like the default size of the window, which modules are loaded, and other stuff.",
   returns = "()",
   type = "function"
  },
  data = {
   childs = {
    ContainerType = {
     childs = {
      data = {
       description = "Return type is ByteData.",
       type = "value"
      },
      string = {
       description = "Return type is string.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    EncodeFormat = {
     childs = {
      base64 = {
       description = "Encode/decode data as base64 binary-to-text encoding.",
       type = "value"
      },
      hex = {
       description = "Encode/decode data as hexadecimal string.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    HashFunction = {
     childs = {
      md5 = {
       description = "MD5 hash algorithm (16 bytes).",
       type = "value"
      },
      sha1 = {
       description = "SHA1 hash algorithm (20 bytes).",
       type = "value"
      },
      sha224 = {
       description = "SHA2 hash algorithm with message digest size of 224 bits (28 bytes).",
       type = "value"
      },
      sha256 = {
       description = "SHA2 hash algorithm with message digest size of 256 bits (32 bytes).",
       type = "value"
      },
      sha384 = {
       description = "SHA2 hash algorithm with message digest size of 384 bits (48 bytes).",
       type = "value"
      },
      sha512 = {
       description = "SHA2 hash algorithm with message digest size of 512 bits (64 bytes).",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    decode = {
     args = "(container: ContainerType, format: EncodeFormat, sourceString: string)",
     description = "Decode Data or a string from any of the EncodeFormats to Data or string.",
     returns = "(decoded: value)",
     type = "function"
    },
    decompress = {
     args = "(container: ContainerType, compressedData: CompressedData)",
     description = "Decompresses a CompressedData or previously compressed string or Data object.",
     returns = "(decompressedData: value)",
     type = "function"
    },
    encode = {
     args = "(container: ContainerType, format: EncodeFormat, sourceString: string, linelength: number)",
     description = "Encode Data or a string to a Data or string in one of the EncodeFormats.",
     returns = "(encoded: value)",
     type = "function"
    },
    getPackedSize = {
     args = "(format: string)",
     description = "Gets the size in bytes that a given format used with love.data.pack will use.\n\nThis function behaves the same as Lua 5.3's string.packsize.",
     returns = "(size: number)",
     type = "function"
    },
    hash = {
     args = "(hashFunction: HashFunction, string: string)",
     description = "Compute the message digest of a string using a specified hash algorithm.",
     returns = "(rawdigest: string)",
     type = "function"
    },
    newByteData = {
     args = "(datastring: string)",
     description = "Creates a new Data object containing arbitrary bytes.\n\nData:getPointer along with LuaJIT's FFI can be used to manipulate the contents of the ByteData object after it has been created.",
     returns = "(bytedata: ByteData)",
     type = "function",
     valuetype = "love.ByteData"
    },
    newDataView = {
     args = "(data: Data, offset: number, size: number)",
     description = "Creates a new Data referencing a subsection of an existing Data object.",
     returns = "(view: Data)",
     type = "function",
     valuetype = "love.Data"
    },
    pack = {
     args = "(container: ContainerType, format: string, v1: value, ...: value)",
     description = "Packs (serializes) simple Lua values.\n\nThis function behaves the same as Lua 5.3's string.pack.",
     returns = "(data: value)",
     type = "function"
    },
    unpack = {
     args = "(format: string, datastring: string, pos: number)",
     description = "Unpacks (deserializes) a byte-string or Data into simple Lua values.\n\nThis function behaves the same as Lua 5.3's string.unpack.",
     returns = "(v1: value, ...: value, index: number)",
     type = "function"
    }
   },
   description = "Provides functionality for creating and transforming data.",
   type = "class"
  },
  directorydropped = {
   args = "(path: string)",
   description = "Callback function triggered when a directory is dragged and dropped onto the window.",
   returns = "()",
   type = "function"
  },
  displayrotated = {
   args = "(index: number, orientation: DisplayOrientation)",
   description = "Called when the device display orientation changed, for example, user rotated their phone 180 degrees.",
   returns = "()",
   type = "function"
  },
  draw = {
   args = "()",
   description = "Callback function used to draw on the screen every frame.",
   returns = "()",
   type = "function"
  },
  errorhandler = {
   args = "(msg: string)",
   description = "The error handler, used to display error messages.",
   returns = "()",
   type = "function"
  },
  event = {
   childs = {
    Event = {
     childs = {
      directorydropped = {
       description = "Directory is dragged and dropped onto the window",
       type = "value"
      },
      f = {
       description = "Window focus gained or lost",
       type = "value"
      },
      filedropped = {
       description = "File is dragged and dropped onto the window.",
       type = "value"
      },
      focus = {
       description = "Window focus gained or lost",
       type = "value"
      },
      gamepadaxis = {
       description = "Joystick's virtual gamepad axis moved",
       type = "value"
      },
      gamepadpressed = {
       description = "Joystick's virtual gamepad button pressed",
       type = "value"
      },
      gamepadreleased = {
       description = "Joystick's virtual gamepad button released",
       type = "value"
      },
      joystickadded = {
       description = "Joystick connected",
       type = "value"
      },
      joystickaxis = {
       description = "Joystick axis motion",
       type = "value"
      },
      joystickhat = {
       description = "Joystick hat pressed",
       type = "value"
      },
      joystickpressed = {
       description = "Joystick pressed",
       type = "value"
      },
      joystickreleased = {
       description = "Joystick released",
       type = "value"
      },
      joystickremoved = {
       description = "Joystick disconnected",
       type = "value"
      },
      jp = {
       description = "Joystick pressed",
       type = "value"
      },
      jr = {
       description = "Joystick released",
       type = "value"
      },
      keypressed = {
       description = "Key pressed",
       type = "value"
      },
      keyreleased = {
       description = "Key released",
       type = "value"
      },
      kp = {
       description = "Key pressed",
       type = "value"
      },
      kr = {
       description = "Key released",
       type = "value"
      },
      lowmemory = {
       description = "Running out of memory on mobile devices system",
       type = "value"
      },
      mousefocus = {
       description = "Window mouse focus gained or lost",
       type = "value"
      },
      mousemoved = {
       description = "Mouse position changed",
       type = "value"
      },
      mousepressed = {
       description = "Mouse pressed",
       type = "value"
      },
      mousereleased = {
       description = "Mouse released",
       type = "value"
      },
      mp = {
       description = "Mouse pressed",
       type = "value"
      },
      mr = {
       description = "Mouse released",
       type = "value"
      },
      q = {
       description = "Quit",
       type = "value"
      },
      quit = {
       description = "Quit",
       type = "value"
      },
      resize = {
       description = "Window size changed by the user",
       type = "value"
      },
      textedited = {
       description = "Candidate text for an IME changed",
       type = "value"
      },
      textinput = {
       description = "User entered text",
       type = "value"
      },
      threaderror = {
       description = "A Lua error has occurred in a thread",
       type = "value"
      },
      touchmoved = {
       description = "Touch press moved inside touch screen",
       type = "value"
      },
      touchpressed = {
       description = "Touch screen touched",
       type = "value"
      },
      touchreleased = {
       description = "Touch screen stop touching",
       type = "value"
      },
      visible = {
       description = "Window is minimized or un-minimized by the user",
       type = "value"
      },
      wheelmoved = {
       description = "Mouse wheel moved",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    poll = {
     args = "()",
     description = "Returns an iterator for messages in the event queue.",
     returns = "(i: function)",
     type = "function"
    },
    pump = {
     args = "()",
     description = "Pump events into the event queue.\n\nThis is a low-level function, and is usually not called by the user, but by love.run.\n\nNote that this does need to be called for any OS to think you're still running,\n\nand if you want to handle OS-generated events at all (think callbacks).",
     returns = "()",
     type = "function"
    },
    push = {
     args = "(n: Event, a: Variant, b: Variant, c: Variant, d: Variant, e: Variant, f: Variant, ...: Variant)",
     description = "Adds an event to the event queue.\n\nFrom 0.10.0 onwards, you may pass an arbitrary amount of arguments with this function, though the default callbacks don't ever use more than six.",
     returns = "()",
     type = "function"
    },
    quit = {
     args = "(exitstatus: number)",
     description = "Adds the quit event to the queue.\n\nThe quit event is a signal for the event handler to close LÖVE. It's possible to abort the exit process with the love.quit callback.",
     returns = "()",
     type = "function"
    },
    wait = {
     args = "()",
     description = "Like love.event.poll(), but blocks until there is an event in the queue.",
     returns = "(n: Event, a: Variant, b: Variant, c: Variant, d: Variant, e: Variant, f: Variant, ...: Variant)",
     type = "function"
    }
   },
   description = "Manages events, like keypresses.",
   type = "class"
  },
  filedropped = {
   args = "(file: DroppedFile)",
   description = "Callback function triggered when a file is dragged and dropped onto the window.",
   returns = "()",
   type = "function"
  },
  filesystem = {
   childs = {
    BufferMode = {
     childs = {
      full = {
       description = "Full buffering. Write and append operations are always buffered until the buffer size limit is reached.",
       type = "value"
      },
      line = {
       description = "Line buffering. Write and append operations are buffered until a newline is output or the buffer size limit is reached.",
       type = "value"
      },
      none = {
       description = "No buffering. The result of write and append operations appears immediately.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    File = {
     childs = {
      flush = {
       args = "()",
       description = "Flushes any buffered written data in the file to the disk.",
       returns = "(success: boolean, err: string)",
       type = "function"
      },
      getBuffer = {
       args = "()",
       description = "Gets the buffer mode of a file.",
       returns = "(mode: BufferMode, size: number)",
       type = "function"
      },
      getFilename = {
       args = "()",
       description = "Gets the filename that the File object was created with. If the file object originated from the love.filedropped callback, the filename will be the full platform-dependent file path.",
       returns = "(filename: string)",
       type = "function"
      },
      getMode = {
       args = "()",
       description = "Gets the FileMode the file has been opened with.",
       returns = "(mode: FileMode)",
       type = "function"
      },
      getSize = {
       args = "()",
       description = "Returns the file size.",
       returns = "(size: number)",
       type = "function"
      },
      isEOF = {
       args = "()",
       description = "Gets whether end-of-file has been reached.",
       returns = "(eof: boolean)",
       type = "function"
      },
      isOpen = {
       args = "()",
       description = "Gets whether the file is open.",
       returns = "(open: boolean)",
       type = "function"
      },
      lines = {
       args = "()",
       description = "Iterate over all the lines in a file.",
       returns = "(iterator: function)",
       type = "function"
      },
      open = {
       args = "(mode: FileMode)",
       description = "Open the file for write, read or append.",
       returns = "(ok: boolean, err: string)",
       type = "function"
      },
      read = {
       args = "(bytes: number)",
       description = "Read a number of bytes from a file.",
       returns = "(contents: string, size: number)",
       type = "function"
      },
      seek = {
       args = "(pos: number)",
       description = "Seek to a position in a file",
       returns = "(success: boolean)",
       type = "function"
      },
      setBuffer = {
       args = "(mode: BufferMode, size: number)",
       description = "Sets the buffer mode for a file opened for writing or appending. Files with buffering enabled will not write data to the disk until the buffer size limit is reached, depending on the buffer mode.\n\nFile:flush will force any buffered data to be written to the disk.",
       returns = "(success: boolean, errorstr: string)",
       type = "function"
      },
      tell = {
       args = "()",
       description = "Returns the position in the file.",
       returns = "(pos: number)",
       type = "function"
      },
      write = {
       args = "(data: string, size: number)",
       description = "Write data to a file.",
       returns = "(success: boolean, err: string)",
       type = "function"
      }
     },
     description = "Represents a file on the filesystem. A function that takes a file path can also take a File.",
     inherits = "love.Object",
     type = "class"
    },
    FileData = {
     childs = {
      getFilename = {
       args = "()",
       description = "Gets the filename of the FileData.",
       returns = "(name: string)",
       type = "function"
      }
     },
     description = "Data representing the contents of a file.",
     inherits = "love.Data, love.Object",
     type = "class"
    },
    FileDecoder = {
     childs = {
      base64 = {
       description = "The data is base64-encoded.",
       type = "value"
      },
      file = {
       description = "The data is unencoded.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    FileMode = {
     childs = {
      a = {
       description = "Open a file for append.",
       type = "value"
      },
      c = {
       description = "Do not open a file (represents a closed file.)",
       type = "value"
      },
      r = {
       description = "Open a file for read.",
       type = "value"
      },
      w = {
       description = "Open a file for write.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    FileType = {
     childs = {
      directory = {
       description = "Directory.",
       type = "value"
      },
      file = {
       description = "Regular file.",
       type = "value"
      },
      other = {
       description = "Something completely different like a device.",
       type = "value"
      },
      symlink = {
       description = "Symbolic link.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    areSymlinksEnabled = {
     args = "()",
     description = "Gets whether love.filesystem follows symbolic links.",
     returns = "(enable: boolean)",
     type = "function"
    },
    createDirectory = {
     args = "(name: string)",
     description = "Recursively creates a directory.\n\nWhen called with 'a/b' it creates both 'a' and 'a/b', if they don't exist already.",
     returns = "(success: boolean)",
     type = "function"
    },
    getAppdataDirectory = {
     args = "()",
     description = "Returns the application data directory (could be the same as getUserDirectory)",
     returns = "(path: string)",
     type = "function"
    },
    getCRequirePath = {
     args = "()",
     description = "Gets the filesystem paths that will be searched for c libraries when require is called.\n\nThe paths string returned by this function is a sequence of path templates separated by semicolons. The argument passed to ''require'' will be inserted in place of any question mark ('?') character in each template (after the dot characters in the argument passed to ''require'' are replaced by directory separators.) Additionally, any occurrence of a double question mark ('??') will be replaced by the name passed to require and the default library extension for the platform.\n\nThe paths are relative to the game's source and save directories, as well as any paths mounted with love.filesystem.mount.",
     returns = "(paths: string)",
     type = "function"
    },
    getDirectoryItems = {
     args = "(dir: string)",
     description = "Returns a table with the names of files and subdirectories in the specified path. The table is not sorted in any way; the order is undefined.\n\nIf the path passed to the function exists in the game and the save directory, it will list the files and directories from both places.",
     returns = "(files: table)",
     type = "function"
    },
    getIdentity = {
     args = "()",
     description = "Gets the write directory name for your game. \n\nNote that this only returns the name of the folder to store your files in, not the full path.",
     returns = "(name: string)",
     type = "function"
    },
    getInfo = {
     args = "(path: string, filtertype: FileType)",
     description = "Gets information about the specified file or directory.",
     returns = "(info: table)",
     type = "function"
    },
    getRealDirectory = {
     args = "(filepath: string)",
     description = "Gets the platform-specific absolute path of the directory containing a filepath.\n\nThis can be used to determine whether a file is inside the save directory or the game's source .love.",
     returns = "(realdir: string)",
     type = "function"
    },
    getRequirePath = {
     args = "()",
     description = "Gets the filesystem paths that will be searched when require is called.\n\nThe paths string returned by this function is a sequence of path templates separated by semicolons. The argument passed to ''require'' will be inserted in place of any question mark ('?') character in each template (after the dot characters in the argument passed to ''require'' are replaced by directory separators.)\n\nThe paths are relative to the game's source and save directories, as well as any paths mounted with love.filesystem.mount.",
     returns = "(paths: string)",
     type = "function"
    },
    getSaveDirectory = {
     args = "()",
     description = "Gets the full path to the designated save directory.\n\nThis can be useful if you want to use the standard io library (or something else) to\n\nread or write in the save directory.",
     returns = "(dir: string)",
     type = "function"
    },
    getSource = {
     args = "()",
     description = "Returns the full path to the the .love file or directory. If the game is fused to the LÖVE executable, then the executable is returned.",
     returns = "(path: string)",
     type = "function"
    },
    getSourceBaseDirectory = {
     args = "()",
     description = "Returns the full path to the directory containing the .love file. If the game is fused to the LÖVE executable, then the directory containing the executable is returned.\n\nIf love.filesystem.isFused is true, the path returned by this function can be passed to love.filesystem.mount, which will make the directory containing the main game (e.g. C:\\Program Files\\coolgame\\) readable by love.filesystem.",
     returns = "(path: string)",
     type = "function"
    },
    getUserDirectory = {
     args = "()",
     description = "Returns the path of the user's directory",
     returns = "(path: string)",
     type = "function"
    },
    getWorkingDirectory = {
     args = "()",
     description = "Gets the current working directory.",
     returns = "(cwd: string)",
     type = "function"
    },
    init = {
     args = "(appname: string)",
     description = "Initializes love.filesystem, will be called internally, so should not be used explicitly.",
     returns = "()",
     type = "function"
    },
    isFused = {
     args = "()",
     description = "Gets whether the game is in fused mode or not.\n\nIf a game is in fused mode, its save directory will be directly in the Appdata directory instead of Appdata/LOVE/. The game will also be able to load C Lua dynamic libraries which are located in the save directory.\n\nA game is in fused mode if the source .love has been fused to the executable (see Game Distribution), or if '--fused' has been given as a command-line argument when starting the game.",
     returns = "(fused: boolean)",
     type = "function"
    },
    lines = {
     args = "(name: string)",
     description = "Iterate over the lines in a file.",
     returns = "(iterator: function)",
     type = "function"
    },
    load = {
     args = "(name: string)",
     description = "Loads a Lua file (but does not run it).",
     returns = "(chunk: function, errormsg: string)",
     type = "function"
    },
    mount = {
     args = "(archive: string, mountpoint: string, appendToPath: boolean)",
     description = "Mounts a zip file or folder in the game's save directory for reading.\n\nIt is also possible to mount love.filesystem.getSourceBaseDirectory if the game is in fused mode.",
     returns = "(success: boolean)",
     type = "function"
    },
    newFile = {
     args = "(filename: string)",
     description = "Creates a new File object. \n\nIt needs to be opened before it can be accessed.",
     returns = "(file: File)",
     type = "function",
     valuetype = "love.filesystem.File"
    },
    newFileData = {
     args = "(contents: string, name: string)",
     description = "Creates a new FileData object.",
     returns = "(data: FileData)",
     type = "function",
     valuetype = "love.filesystem.FileData"
    },
    read = {
     args = "(name: string, size: number)",
     description = "Read the contents of a file.",
     returns = "(contents: string, size: number, contents: nil, error: string)",
     type = "function"
    },
    remove = {
     args = "(name: string)",
     description = "Removes a file or empty directory.",
     returns = "(success: boolean)",
     type = "function"
    },
    setCRequirePath = {
     args = "(paths: string)",
     description = "Sets the filesystem paths that will be searched for c libraries when require is called.\n\nThe paths string returned by this function is a sequence of path templates separated by semicolons. The argument passed to ''require'' will be inserted in place of any question mark ('?') character in each template (after the dot characters in the argument passed to ''require'' are replaced by directory separators.) Additionally, any occurrence of a double question mark ('??') will be replaced by the name passed to require and the default library extension for the platform.\n\nThe paths are relative to the game's source and save directories, as well as any paths mounted with love.filesystem.mount.",
     returns = "()",
     type = "function"
    },
    setIdentity = {
     args = "(name: string)",
     description = "Sets the write directory for your game. \n\nNote that you can only set the name of the folder to store your files in, not the location.",
     returns = "()",
     type = "function"
    },
    setRequirePath = {
     args = "(paths: string)",
     description = "Sets the filesystem paths that will be searched when require is called.\n\nThe paths string given to this function is a sequence of path templates separated by semicolons. The argument passed to ''require'' will be inserted in place of any question mark ('?') character in each template (after the dot characters in the argument passed to ''require'' are replaced by directory separators.)\n\nThe paths are relative to the game's source and save directories, as well as any paths mounted with love.filesystem.mount.",
     returns = "()",
     type = "function"
    },
    setSource = {
     args = "(path: string)",
     description = "Sets the source of the game, where the code is present. This function can only be called once, and is normally automatically done by LÖVE.",
     returns = "()",
     type = "function"
    },
    setSymlinksEnabled = {
     args = "(enable: boolean)",
     description = "Sets whether love.filesystem follows symbolic links. It is enabled by default in version 0.10.0 and newer, and disabled by default in 0.9.2.",
     returns = "()",
     type = "function"
    },
    unmount = {
     args = "(archive: string)",
     description = "Unmounts a zip file or folder previously mounted for reading with love.filesystem.mount.",
     returns = "(success: boolean)",
     type = "function"
    },
    write = {
     args = "(name: string, data: string, size: number)",
     description = "Write data to a file in the save directory. If the file existed already, it will be completely replaced by the new contents.",
     returns = "(success: boolean, message: string)",
     type = "function"
    }
   },
   description = "Provides an interface to the user's filesystem.",
   type = "class"
  },
  focus = {
   args = "(focus: boolean)",
   description = "Callback function triggered when window receives or loses focus.",
   returns = "()",
   type = "function"
  },
  gamepadaxis = {
   args = "(joystick: Joystick, axis: GamepadAxis, value: number)",
   description = "Called when a Joystick's virtual gamepad axis is moved.",
   returns = "()",
   type = "function"
  },
  gamepadpressed = {
   args = "(joystick: Joystick, button: GamepadButton)",
   description = "Called when a Joystick's virtual gamepad button is pressed.",
   returns = "()",
   type = "function"
  },
  gamepadreleased = {
   args = "(joystick: Joystick, button: GamepadButton)",
   description = "Called when a Joystick's virtual gamepad button is released.",
   returns = "()",
   type = "function"
  },
  getVersion = {
   args = "()",
   description = "Gets the current running version of LÖVE.",
   returns = "(major: number, minor: number, revision: number, codename: string)",
   type = "function"
  },
  graphics = {
   childs = {
    AlignMode = {
     childs = {
      center = {
       description = "Align text center.",
       type = "value"
      },
      justify = {
       description = "Align text both left and right.",
       type = "value"
      },
      left = {
       description = "Align text left.",
       type = "value"
      },
      right = {
       description = "Align text right.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    ArcType = {
     childs = {
      closed = {
       description = "The arc circle's two end-points are connected to each other.",
       type = "value"
      },
      open = {
       description = "The arc circle's two end-points are unconnected when the arc is drawn as a line. Behaves like the \"closed\" arc type when the arc is drawn in filled mode.",
       type = "value"
      },
      pie = {
       description = "The arc is drawn like a slice of pie, with the arc circle connected to the center at its end-points.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    AreaSpreadDistribution = {
     childs = {
      borderellipse = {
       description = "Distribution in an ellipse with particles spawning at the edges of the ellipse.",
       type = "value"
      },
      borderrectangle = {
       description = "Distribution in a rectangle with particles spawning at the edges of the rectangle.",
       type = "value"
      },
      ellipse = {
       description = "Uniform distribution in an ellipse.",
       type = "value"
      },
      none = {
       description = "No distribution - area spread is disabled.",
       type = "value"
      },
      normal = {
       description = "Normal (gaussian) distribution.",
       type = "value"
      },
      uniform = {
       description = "Uniform distribution.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    BlendAlphaMode = {
     childs = {
      alphamultiply = {
       description = "The RGB values of what's drawn are multiplied by the alpha values of those colors during blending. This is the default alpha mode.",
       type = "value"
      },
      premultiplied = {
       description = "The RGB values of what's drawn are '''not''' multiplied by the alpha values of those colors during blending. For most blend modes to work correctly with this alpha mode, the colors of a drawn object need to have had their RGB values multiplied by their alpha values at some point previously (\"premultiplied alpha\").",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    BlendMode = {
     childs = {
      add = {
       description = "The pixel colors of what's drawn are added to the pixel colors already on the screen. The alpha of the screen is not modified.",
       type = "value"
      },
      additive = {
       description = "Additive blend mode.",
       type = "value"
      },
      alpha = {
       description = "Alpha blending (normal). The alpha of what's drawn determines its opacity.",
       type = "value"
      },
      darken = {
       description = "The pixel colors of what's drawn are compared to the existing pixel colors, and the smaller of the two values for each color component is used. Only works when the \"premultiplied\" BlendAlphaMode is used in love.graphics.setBlendMode.",
       type = "value"
      },
      lighten = {
       description = "The pixel colors of what's drawn are compared to the existing pixel colors, and the larger of the two values for each color component is used. Only works when the \"premultiplied\" BlendAlphaMode is used in love.graphics.setBlendMode.",
       type = "value"
      },
      multiplicative = {
       description = "Multiply blend mode.",
       type = "value"
      },
      multiply = {
       description = "The pixel colors of what's drawn are multiplied with the pixel colors already on the screen (darkening them). The alpha of drawn objects is multiplied with the alpha of the screen rather than determining how much the colors on the screen are affected, even when the \"alphamultiply\" BlendAlphaMode is used.",
       type = "value"
      },
      premultiplied = {
       description = "Premultiplied alpha blend mode.",
       type = "value"
      },
      replace = {
       description = "The colors of what's drawn completely replace what was on the screen, with no additional blending. The BlendAlphaMode specified in love.graphics.setBlendMode still affects what happens.",
       type = "value"
      },
      screen = {
       description = "'Screen' blending.",
       type = "value"
      },
      subtract = {
       description = "The pixel colors of what's drawn are subtracted from the pixel colors already on the screen. The alpha of the screen is not modified.",
       type = "value"
      },
      subtractive = {
       description = "Subtractive blend mode.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    Canvas = {
     childs = {
      getDimensions = {
       args = "()",
       description = "Gets the width and height of the Canvas.",
       returns = "(width: number, height: number)",
       type = "function"
      },
      getFilter = {
       args = "()",
       description = "Gets the filter mode of the Canvas.",
       returns = "(min: FilterMode, mag: FilterMode)",
       type = "function"
      },
      getHeight = {
       args = "()",
       description = "Gets the height of the Canvas.",
       returns = "(height: number)",
       type = "function"
      },
      getMSAA = {
       args = "()",
       description = "Gets the number of multisample antialiasing (MSAA) samples used when drawing to the Canvas.\n\nThis may be different than the number used as an argument to love.graphics.newCanvas if the system running LÖVE doesn't support that number.",
       returns = "(samples: number)",
       type = "function"
      },
      getMipmapMode = {
       args = "()",
       description = "Gets the MipmapMode this Canvas was created with.",
       returns = "(mode: MipmapMode)",
       type = "function"
      },
      getWidth = {
       args = "()",
       description = "Gets the width of the Canvas.",
       returns = "(width: number)",
       type = "function"
      },
      getWrap = {
       args = "()",
       description = "Gets the wrapping properties of a Canvas.\n\nThis function returns the currently set horizontal and vertical wrapping modes for the Canvas.",
       returns = "(horiz: WrapMode, vert: WrapMode)",
       type = "function"
      },
      newImageData = {
       args = "(slice: number, mipmap: number, x: number, y: number, width: number, height: number)",
       description = "Generates ImageData from the contents of the Canvas.",
       returns = "(data: ImageData)",
       type = "function",
       valuetype = "love.image.ImageData"
      },
      renderTo = {
       args = "(func: function)",
       description = "Render to the Canvas using a function.\n\nThis is a shortcut to love.graphics.setCanvas:\n\ncanvas:renderTo( func )\n\nis the same as\n\nlove.graphics.setCanvas( canvas )\n\nfunc()\n\nlove.graphics.setCanvas()",
       returns = "()",
       type = "function"
      },
      setFilter = {
       args = "(min: FilterMode, mag: FilterMode)",
       description = "Sets the filter mode of the Canvas.",
       returns = "()",
       type = "function"
      },
      setWrap = {
       args = "(horiz: WrapMode, vert: WrapMode)",
       description = "Sets the wrapping properties of a Canvas.\n\nThis function sets the way the edges of a Canvas are treated if it is scaled or rotated. If the WrapMode is set to 'clamp', the edge will not be interpolated. If set to 'repeat', the edge will be interpolated with the pixels on the opposing side of the framebuffer.",
       returns = "()",
       type = "function"
      }
     },
     description = "A Canvas is used for off-screen rendering. Think of it as an invisible screen that you can draw to, but that will not be visible until you draw it to the actual visible screen. It is also known as \"render to texture\".\n\nBy drawing things that do not change position often (such as background items) to the Canvas, and then drawing the entire Canvas instead of each item,  you can reduce the number of draw operations performed each frame.\n\nIn versions prior to love.graphics.isSupported(\"canvas\") could be used to check for support at runtime.",
     inherits = "love.graphics.Texture, love.Drawable, love.Object",
     type = "class"
    },
    CanvasMipmapMode = {
     childs = {
      auto = {
       description = "Automatically generate mipmap.",
       type = "value"
      },
      manual = {
       description = "Let user manually generate mipmap.",
       type = "value"
      },
      none = {
       description = "Do not enable mipmap.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    CompareMode = {
     childs = {
      always = {
       description = "Objects will always be drawn. Effectively disables the depth or stencil test.",
       type = "value"
      },
      never = {
       description = "Objects will never be drawn.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    CullMode = {
     childs = {
      back = {
       description = "Back-facing triangles in Meshes are culled (not rendered). The vertex order of a triangle determines whether it is back- or front-facing.",
       type = "value"
      },
      front = {
       description = "Front-facing triangles in Meshes are culled.",
       type = "value"
      },
      none = {
       description = "Both back- and front-facing triangles in Meshes are rendered.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    DrawMode = {
     childs = {
      fill = {
       description = "Draw filled shape.",
       type = "value"
      },
      line = {
       description = "Draw outlined shape.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    FilterMode = {
     childs = {
      linear = {
       description = "Scale image with linear interpolation.",
       type = "value"
      },
      nearest = {
       description = "Scale image with nearest neighbor interpolation.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    Font = {
     childs = {
      getBaseline = {
       args = "()",
       description = "Gets the baseline of the Font.\n\nMost scripts share the notion of a baseline: an imaginary horizontal line on which characters rest. In some scripts, parts of glyphs lie below the baseline.",
       returns = "(baseline: number)",
       type = "function"
      },
      getDPIScale = {
       args = "()",
       description = "Gets the DPI scale factor of the Font.\n\nThe DPI scale factor represents relative pixel density. A DPI scale factor of 2 means the font's glyphs have twice the pixel density in each dimension (4 times as many pixels in the same area) compared to a font with a DPI scale factor of 1.\n\nThe font size of TrueType fonts is scaled internally by the font's specified DPI scale factor. By default, LÖVE uses the screen's DPI scale factor when creating TrueType fonts.",
       returns = "(dpiscale: number)",
       type = "function"
      },
      getDescent = {
       args = "()",
       description = "Gets the descent of the Font.\n\nThe descent spans the distance between the baseline and the lowest descending glyph in a typeface.",
       returns = "(descent: number)",
       type = "function"
      },
      getFilter = {
       args = "()",
       description = "Gets the filter mode for a font.",
       returns = "(min: FilterMode, mag: FilterMode, anisotropy: number)",
       type = "function"
      },
      getHeight = {
       args = "()",
       description = "Gets the height of the Font.\n\nThe height of the font is the size including any spacing; the height which it will need.",
       returns = "(height: number)",
       type = "function"
      },
      getLineHeight = {
       args = "()",
       description = "Gets the line height.\n\nThis will be the value previously set by Font:setLineHeight, or 1.0 by default.",
       returns = "(height: number)",
       type = "function"
      },
      getWidth = {
       args = "(text: string)",
       description = "Determines the maximum width (accounting for newlines) taken by the given string.",
       returns = "(width: number)",
       type = "function"
      },
      getWrap = {
       args = "(text: string, wraplimit: number)",
       description = "Gets formatting information for text, given a wrap limit.\n\nThis function accounts for newlines correctly (i.e. '\\n').",
       returns = "(width: number, wrappedtext: table)",
       type = "function"
      },
      hasGlyphs = {
       args = "(text: string)",
       description = "Gets whether the Font can render a character or string.",
       returns = "(hasglyph: boolean)",
       type = "function"
      },
      setFallbacks = {
       args = "(fallbackfont1: Font, ...: Font)",
       description = "Sets the fallback fonts. When the Font doesn't contain a glyph, it will substitute the glyph from the next subsequent fallback Fonts. This is akin to setting a 'font stack' in Cascading Style Sheets (CSS).",
       returns = "()",
       type = "function"
      },
      setFilter = {
       args = "(min: FilterMode, mag: FilterMode, anisotropy: number)",
       description = "Sets the filter mode for a font.",
       returns = "()",
       type = "function"
      },
      setLineHeight = {
       args = "(height: number)",
       description = "Sets the line height.\n\nWhen rendering the font in lines the actual height will be determined by the line height multiplied by the height of the font. The default is 1.0.",
       returns = "()",
       type = "function"
      }
     },
     description = "Defines the shape of characters that can be drawn onto the screen.",
     inherits = "love.Object",
     type = "class"
    },
    GraphicsFeature = {
     childs = {
      bc5 = {
       description = "Support for BC4 and BC5 compressed images. Use love.graphics.getCompressedImageFormats instead.",
       type = "value"
      },
      canvas = {
       description = "Support for Canvas.",
       type = "value"
      },
      clampzero = {
       description = "Whether the \"clampzero\" WrapMode is supported.",
       type = "value"
      },
      dxt = {
       description = "Support for DXT compressed images (see CompressedFormat.) Use love.graphics.getCompressedImageFormats instead.",
       type = "value"
      },
      fullnpot = {
       description = "Whether textures with non-power-of-two dimensions can use mipmapping and the 'repeat' WrapMode.",
       type = "value"
      },
      glsl3 = {
       description = "Whether GLSL 3 Shaders can be used.",
       type = "value"
      },
      hdrcanvas = {
       description = "Support for HDR Canvases. Use love.graphics.getCanvasFormats instead.",
       type = "value"
      },
      instancing = {
       description = "Whether mesh instancing is supported.",
       type = "value"
      },
      lighten = {
       description = "Whether the \"lighten\" and \"darken\" BlendModes are supported.",
       type = "value"
      },
      mipmap = {
       description = "Support for Mipmaps.",
       type = "value"
      },
      multicanvas = {
       description = "Support for simultaneous rendering to at least 4 canvases at once, with love.graphics.setCanvas. Use love.graphics.getSystemLimits instead.",
       type = "value"
      },
      multicanvasformats = {
       description = "Whether multiple formats can be used in the same love.graphics.setCanvas call.",
       type = "value"
      },
      npot = {
       description = "Support for textures with non-power-of-two sizes. See PO2 Syndrome. ",
       type = "value"
      },
      pixeleffect = {
       description = "Support for PixelEffects.",
       type = "value"
      },
      pixelshaderhighp = {
       description = "Whether pixel shaders can use \"highp\" 32 bit floating point numbers (as opposed to just 16 bit or lower precision).",
       type = "value"
      },
      shader = {
       description = "Support for Shaders.",
       type = "value"
      },
      shaderderivatives = {
       description = "Whether shaders can use the dFdx, dFdy, and fwidth functions for computing derivatives.",
       type = "value"
      },
      srgb = {
       description = "Support for gamma-correct rendering with the srgb window flag in Canvases and Images.",
       type = "value"
      },
      subtractive = {
       description = "Support for the subtractive blend mode.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    GraphicsLimit = {
     childs = {
      anisotropy = {
       description = "The maximum amount of anisotropic filtering. Texture:setMipmapFilter internally clamps the given anisotropy value to the system's limit.",
       type = "value"
      },
      canvasfsaa = {
       description = "The maximum number of antialiasing samples for a Canvas.",
       type = "value"
      },
      canvasmsaa = {
       description = "The maximum number of antialiasing samples for a Canvas.",
       type = "value"
      },
      cubetexturesize = {
       description = "The maximum width or height of a Cubemap texture.",
       type = "value"
      },
      multicanvas = {
       description = "The maximum number of simultaneously active canvases (via love.graphics.setCanvas.)",
       type = "value"
      },
      pointsize = {
       description = "The maximum size of points.",
       type = "value"
      },
      texturelayers = {
       description = "The maximum number of layers in an Array texture.",
       type = "value"
      },
      texturesize = {
       description = "The maximum width or height of Images and Canvases.",
       type = "value"
      },
      volumetexturesize = {
       description = "The maximum width, height, or depth of a Volume texture.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    Image = {
     childs = {
      getFilter = {
       args = "()",
       description = "Gets the filter mode for an image.",
       returns = "(min: FilterMode, mag: FilterMode)",
       type = "function"
      },
      getFlags = {
       args = "()",
       description = "Gets the flags used when the image was created.",
       returns = "(flags: table)",
       type = "function"
      },
      getHeight = {
       args = "()",
       description = "Gets the height of the Image.",
       returns = "(height: number)",
       type = "function"
      },
      getWidth = {
       args = "()",
       description = "Gets the width of the Image.",
       returns = "(width: number)",
       type = "function"
      },
      getWrap = {
       args = "()",
       description = "Gets the wrapping properties of an Image.\n\nThis function returns the currently set horizontal and vertical wrapping modes for the image.",
       returns = "(horiz: WrapMode, vert: WrapMode)",
       type = "function"
      },
      replacePixels = {
       args = "(data: ImageData, slice: number, mipmap: number, x: number, y: number, reloadmipmaps: boolean)",
       description = "Replace the contents of an Image.",
       returns = "()",
       type = "function"
      },
      setFilter = {
       args = "(min: FilterMode, mag: FilterMode)",
       description = "Sets the filter mode for an image.",
       returns = "()",
       type = "function"
      },
      setWrap = {
       args = "(horiz: WrapMode, vert: WrapMode)",
       description = "Sets the wrapping properties of an Image.\n\nThis function sets the way an Image is repeated when it is drawn with a  Quad that is larger than the image's extent. An image may be clamped or set to repeat in both horizontal and vertical directions. Clamped images appear only once, but repeated ones repeat as many times as there is room in the Quad.\n\nN.B. If you use a Quad that is larger than the image extent and do not use repeated tiling, there may be an unwanted visual effect of the image stretching all the way to fill the Quad. If this is the case, setting Image:setWrap('repeat', 'repeat') for all the images to be repeated, and using Quads of appropriate size will result in the best visual appearance.",
       returns = "()",
       type = "function"
      }
     },
     description = "Drawable image type.",
     inherits = "love.graphics.Texture, love.Drawable, love.Object",
     type = "class"
    },
    LineJoin = {
     childs = {
      bevel = {
       description = "Flattens the point where line segments join together.",
       type = "value"
      },
      miter = {
       description = "The ends of the line segments beveled in an angle so that they join seamlessly.",
       type = "value"
      },
      none = {
       description = "No cap applied to the ends of the line segments.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    LineStyle = {
     childs = {
      rough = {
       description = "Draw rough lines.",
       type = "value"
      },
      smooth = {
       description = "Draw smooth lines.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    Mesh = {
     childs = {
      attachAttribute = {
       args = "(name: string, mesh: Mesh)",
       description = "Attaches a vertex attribute from a different Mesh onto this Mesh, for use when drawing. This can be used to share vertex attribute data between several different Meshes.",
       returns = "()",
       type = "function"
      },
      detachAttribute = {
       args = "(name: string)",
       description = "Removes a previously attached vertex attribute from this Mesh.",
       returns = "(success: boolean)",
       type = "function"
      },
      getDrawMode = {
       args = "()",
       description = "Gets the mode used when drawing the Mesh.",
       returns = "(mode: MeshDrawMode)",
       type = "function"
      },
      getDrawRange = {
       args = "()",
       description = "Gets the range of vertices used when drawing the Mesh.",
       returns = "(min: number, max: number)",
       type = "function"
      },
      getTexture = {
       args = "()",
       description = "Gets the texture (Image or Canvas) used when drawing the Mesh.",
       returns = "(texture: Texture)",
       type = "function",
       valuetype = "love.graphics.Texture"
      },
      getVertex = {
       args = "(index: number)",
       description = "Gets the properties of a vertex in the Mesh.\n\nIn versions prior to 11.0, color and byte component values were within the range of 0 to 255 instead of 0 to 1.",
       returns = "(attributecomponent: number, ...: number)",
       type = "function"
      },
      getVertexAttribute = {
       args = "(vertexindex: number, attributeindex: number)",
       description = "Gets the properties of a specific attribute within a vertex in the Mesh.\n\nMeshes without a custom vertex format specified in love.graphics.newMesh have position as their first attribute, texture coordinates as their second attribute, and color as their third attribute.",
       returns = "(value1: number, value2: number, ...: number)",
       type = "function"
      },
      getVertexCount = {
       args = "()",
       description = "Gets the total number of vertices in the Mesh.",
       returns = "(count: number)",
       type = "function"
      },
      getVertexFormat = {
       args = "()",
       description = "Gets the vertex format that the Mesh was created with.",
       returns = "(format: table)",
       type = "function"
      },
      getVertexMap = {
       args = "()",
       description = "Gets the vertex map for the Mesh. The vertex map describes the order in which the vertices are used when the Mesh is drawn. The vertices, vertex map, and mesh draw mode work together to determine what exactly is displayed on the screen.\n\nIf no vertex map has been set previously via Mesh:setVertexMap, then this function will return nil in LÖVE 0.10.0+, or an empty table in 0.9.2 and older.",
       returns = "(map: table)",
       type = "function"
      },
      isAttributeEnabled = {
       args = "(name: string)",
       description = "Gets whether a specific vertex attribute in the Mesh is enabled. Vertex data from disabled attributes is not used when drawing the Mesh.",
       returns = "(enabled: boolean)",
       type = "function"
      },
      setAttributeEnabled = {
       args = "(name: string, enable: boolean)",
       description = "Enables or disables a specific vertex attribute in the Mesh. Vertex data from disabled attributes is not used when drawing the Mesh.",
       returns = "()",
       type = "function"
      },
      setDrawMode = {
       args = "(mode: MeshDrawMode)",
       description = "Sets the mode used when drawing the Mesh.",
       returns = "()",
       type = "function"
      },
      setDrawRange = {
       args = "(start: number, count: number)",
       description = "Restricts the drawn vertices of the Mesh to a subset of the total.",
       returns = "()",
       type = "function"
      },
      setTexture = {
       args = "(texture: Texture)",
       description = "Sets the texture (Image or Canvas) used when drawing the Mesh.",
       returns = "()",
       type = "function"
      },
      setVertex = {
       args = "(index: number, attributecomponent: number, ...: number)",
       description = "Sets the properties of a vertex in the Mesh.\n\nIn versions prior to 11.0, color and byte component values were within the range of 0 to 255 instead of 0 to 1.",
       returns = "()",
       type = "function"
      },
      setVertexAttribute = {
       args = "(vertexindex: number, attributeindex: number, value1: number, value2: number, ...: number)",
       description = "Sets the properties of a specific attribute within a vertex in the Mesh.\n\nMeshes without a custom vertex format specified in love.graphics.newMesh have position as their first attribute, texture coordinates as their second attribute, and color as their third attribute.",
       returns = "()",
       type = "function"
      },
      setVertexMap = {
       args = "(map: table)",
       description = "Sets the vertex map for the Mesh. The vertex map describes the order in which the vertices are used when the Mesh is drawn. The vertices, vertex map, and mesh draw mode work together to determine what exactly is displayed on the screen.\n\nThe vertex map allows you to re-order or reuse vertices when drawing without changing the actual vertex parameters or duplicating vertices. It is especially useful when combined with different Mesh Draw Modes.",
       returns = "()",
       type = "function"
      },
      setVertices = {
       args = "(vertices: table, startvertex: number)",
       description = "Replaces a range of vertices in the Mesh with new ones. The total number of vertices in a Mesh cannot be changed after it has been created. This is often more efficient than calling Mesh:setVertex in a loop.",
       returns = "()",
       type = "function"
      }
     },
     description = "A 2D polygon mesh used for drawing arbitrary textured shapes.",
     inherits = "love.Drawable, love.Object",
     type = "class"
    },
    MeshDrawMode = {
     childs = {
      fan = {
       description = "The vertices create a \"fan\" shape with the first vertex acting as the hub point. Can be easily used to draw simple convex polygons.",
       type = "value"
      },
      points = {
       description = "The vertices are drawn as unconnected points (see love.graphics.setPointSize.)",
       type = "value"
      },
      strip = {
       description = "The vertices create a series of connected triangles using vertices 1, 2, 3, then 3, 2, 4 (note the order), then 3, 4, 5, and so on.",
       type = "value"
      },
      triangles = {
       description = "The vertices create unconnected triangles.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    ParticleInsertMode = {
     childs = {
      bottom = {
       description = "Particles are inserted at the bottom of the ParticleSystem's list of particles.",
       type = "value"
      },
      random = {
       description = "Particles are inserted at random positions in the ParticleSystem's list of particles.",
       type = "value"
      },
      top = {
       description = "Particles are inserted at the top of the ParticleSystem's list of particles.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    ParticleSystem = {
     childs = {
      emit = {
       args = "(numparticles: number)",
       description = "Emits a burst of particles from the particle emitter.",
       returns = "()",
       type = "function"
      },
      getBufferSize = {
       args = "()",
       description = "Gets the maximum number of particles the ParticleSystem can have at once.",
       returns = "(size: number)",
       type = "function"
      },
      getColors = {
       args = "()",
       description = "Gets the series of colors applied to the particle sprite.\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.",
       returns = "(r1: number, g1: number, b1: number, a1: number, r2: number, g2: number, b2: number, a2: number, r8: number, g8: number, b8: number, a8: number)",
       type = "function"
      },
      getCount = {
       args = "()",
       description = "Gets the number of particles that are currently in the system.",
       returns = "(count: number)",
       type = "function"
      },
      getDirection = {
       args = "()",
       description = "Gets the direction of the particle emitter (in radians).",
       returns = "(direction: number)",
       type = "function"
      },
      getEmissionArea = {
       args = "()",
       description = "Gets the area-based spawn parameters for the particles.",
       returns = "(distribution: AreaSpreadDistribution, dx: number, dy: number, angle: number, directionRelativeToCenter: boolean)",
       type = "function"
      },
      getEmissionRate = {
       args = "()",
       description = "Gets the amount of particles emitted per second.",
       returns = "(rate: number)",
       type = "function"
      },
      getEmitterLifetime = {
       args = "()",
       description = "Gets how long the particle system will emit particles (if -1 then it emits particles forever).",
       returns = "(life: number)",
       type = "function"
      },
      getInsertMode = {
       args = "()",
       description = "Gets the mode used when the ParticleSystem adds new particles.",
       returns = "(mode: ParticleInsertMode)",
       type = "function"
      },
      getLinearAcceleration = {
       args = "()",
       description = "Gets the linear acceleration (acceleration along the x and y axes) for particles.\n\nEvery particle created will accelerate along the x and y axes between xmin,ymin and xmax,ymax.",
       returns = "(xmin: number, ymin: number, xmax: number, ymax: number)",
       type = "function"
      },
      getLinearDamping = {
       args = "()",
       description = "Gets the amount of linear damping (constant deceleration) for particles.",
       returns = "(min: number, max: number)",
       type = "function"
      },
      getOffset = {
       args = "()",
       description = "Gets the particle image's draw offset.",
       returns = "(ox: number, oy: number)",
       type = "function"
      },
      getParticleLifetime = {
       args = "()",
       description = "Gets the lifetime of the particles.",
       returns = "(min: number, max: number)",
       type = "function"
      },
      getPosition = {
       args = "()",
       description = "Gets the position of the emitter.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      getQuads = {
       args = "()",
       description = "Gets the series of Quads used for the particle sprites.",
       returns = "(quads: table)",
       type = "function"
      },
      getRadialAcceleration = {
       args = "()",
       description = "Gets the radial acceleration (away from the emitter).",
       returns = "(min: number, max: number)",
       type = "function"
      },
      getRotation = {
       args = "()",
       description = "Gets the rotation of the image upon particle creation (in radians).",
       returns = "(min: number, max: number)",
       type = "function"
      },
      getSizeVariation = {
       args = "()",
       description = "Gets the amount of size variation (0 meaning no variation and 1 meaning full variation between start and end).",
       returns = "(variation: number)",
       type = "function"
      },
      getSizes = {
       args = "()",
       description = "Gets the series of sizes by which the sprite is scaled. 1.0 is normal size. The particle system will interpolate between each size evenly over the particle's lifetime.",
       returns = "(size1: number, size2: number, size8: number)",
       type = "function"
      },
      getSpeed = {
       args = "()",
       description = "Gets the speed of the particles.",
       returns = "(min: number, max: number)",
       type = "function"
      },
      getSpin = {
       args = "()",
       description = "Gets the spin of the sprite.",
       returns = "(min: number, max: number, variation: number)",
       type = "function"
      },
      getSpinVariation = {
       args = "()",
       description = "Gets the amount of spin variation (0 meaning no variation and 1 meaning full variation between start and end).",
       returns = "(variation: number)",
       type = "function"
      },
      getSpread = {
       args = "()",
       description = "Gets the amount of directional spread of the particle emitter (in radians).",
       returns = "(spread: number)",
       type = "function"
      },
      getTangentialAcceleration = {
       args = "()",
       description = "Gets the tangential acceleration (acceleration perpendicular to the particle's direction).",
       returns = "(min: number, max: number)",
       type = "function"
      },
      getTexture = {
       args = "()",
       description = "Gets the texture (Image or Canvas) used for the particles.",
       returns = "(texture: Texture)",
       type = "function",
       valuetype = "love.graphics.Texture"
      },
      hasRelativeRotation = {
       args = "()",
       description = "Gets whether particle angles and rotations are relative to their velocities. If enabled, particles are aligned to the angle of their velocities and rotate relative to that angle.",
       returns = "(enable: boolean)",
       type = "function"
      },
      isActive = {
       args = "()",
       description = "Checks whether the particle system is actively emitting particles.",
       returns = "(active: boolean)",
       type = "function"
      },
      isPaused = {
       args = "()",
       description = "Checks whether the particle system is paused.",
       returns = "(paused: boolean)",
       type = "function"
      },
      isStopped = {
       args = "()",
       description = "Checks whether the particle system is stopped.",
       returns = "(stopped: boolean)",
       type = "function"
      },
      moveTo = {
       args = "(x: number, y: number)",
       description = "Moves the position of the emitter. This results in smoother particle spawning behaviour than if ParticleSystem:setPosition is used every frame.",
       returns = "()",
       type = "function"
      },
      pause = {
       args = "()",
       description = "Pauses the particle emitter.",
       returns = "()",
       type = "function"
      },
      reset = {
       args = "()",
       description = "Resets the particle emitter, removing any existing particles and resetting the lifetime counter.",
       returns = "()",
       type = "function"
      },
      setBufferSize = {
       args = "(size: number)",
       description = "Sets the size of the buffer (the max allowed amount of particles in the system).",
       returns = "()",
       type = "function"
      },
      setColors = {
       args = "(r1: number, g1: number, b1: number, a1: number, r2: number, g2: number, b2: number, a2: number, r8: number, g8: number, b8: number, a8: number)",
       description = "Sets a series of colors to apply to the particle sprite. The particle system will interpolate between each color evenly over the particle's lifetime.\n\nArguments can be passed in groups of four, representing the components of the desired RGBA value, or as tables of RGBA component values, with a default alpha value of 1 if only three values are given. At least one color must be specified. A maximum of eight may be used.\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.",
       returns = "()",
       type = "function"
      },
      setDirection = {
       args = "(direction: number)",
       description = "Sets the direction the particles will be emitted in.",
       returns = "()",
       type = "function"
      },
      setEmissionArea = {
       args = "(distribution: AreaSpreadDistribution, dx: number, dy: number, angle: number, directionRelativeToCenter: boolean)",
       description = "Sets area-based spawn parameters for the particles. Newly created particles will spawn in an area around the emitter based on the parameters to this function.",
       returns = "()",
       type = "function"
      },
      setEmissionRate = {
       args = "(rate: number)",
       description = "Sets the amount of particles emitted per second.",
       returns = "()",
       type = "function"
      },
      setEmitterLifetime = {
       args = "(life: number)",
       description = "Sets how long the particle system should emit particles (if -1 then it emits particles forever).",
       returns = "()",
       type = "function"
      },
      setInsertMode = {
       args = "(mode: ParticleInsertMode)",
       description = "Sets the mode to use when the ParticleSystem adds new particles.",
       returns = "()",
       type = "function"
      },
      setLinearAcceleration = {
       args = "(xmin: number, ymin: number, xmax: number, ymax: number)",
       description = "Sets the linear acceleration (acceleration along the x and y axes) for particles.\n\nEvery particle created will accelerate along the x and y axes between xmin,ymin and xmax,ymax.",
       returns = "()",
       type = "function"
      },
      setLinearDamping = {
       args = "(min: number, max: number)",
       description = "Sets the amount of linear damping (constant deceleration) for particles.",
       returns = "()",
       type = "function"
      },
      setOffset = {
       args = "(x: number, y: number)",
       description = "Set the offset position which the particle sprite is rotated around.\n\nIf this function is not used, the particles rotate around their center.",
       returns = "()",
       type = "function"
      },
      setParticleLifetime = {
       args = "(min: number, max: number)",
       description = "Sets the lifetime of the particles.",
       returns = "()",
       type = "function"
      },
      setPosition = {
       args = "(x: number, y: number)",
       description = "Sets the position of the emitter.",
       returns = "()",
       type = "function"
      },
      setQuads = {
       args = "(quad1: Quad, quad2: Quad)",
       description = "Sets a series of Quads to use for the particle sprites. Particles will choose a Quad from the list based on the particle's current lifetime, allowing for the use of animated sprite sheets with ParticleSystems.",
       returns = "()",
       type = "function"
      },
      setRadialAcceleration = {
       args = "(min: number, max: number)",
       description = "Set the radial acceleration (away from the emitter).",
       returns = "()",
       type = "function"
      },
      setRelativeRotation = {
       args = "(enable: boolean)",
       description = "Sets whether particle angles and rotations are relative to their velocities. If enabled, particles are aligned to the angle of their velocities and rotate relative to that angle.",
       returns = "()",
       type = "function"
      },
      setRotation = {
       args = "(min: number, max: number)",
       description = "Sets the rotation of the image upon particle creation (in radians).",
       returns = "()",
       type = "function"
      },
      setSizeVariation = {
       args = "(variation: number)",
       description = "Sets the amount of size variation (0 meaning no variation and 1 meaning full variation between start and end).",
       returns = "()",
       type = "function"
      },
      setSizes = {
       args = "(size1: number, size2: number, size8: number)",
       description = "Sets a series of sizes by which to scale a particle sprite. 1.0 is normal size. The particle system will interpolate between each size evenly over the particle's lifetime.\n\nAt least one size must be specified. A maximum of eight may be used.",
       returns = "()",
       type = "function"
      },
      setSpeed = {
       args = "(min: number, max: number)",
       description = "Sets the speed of the particles.",
       returns = "()",
       type = "function"
      },
      setSpin = {
       args = "(min: number, max: number)",
       description = "Sets the spin of the sprite.",
       returns = "()",
       type = "function"
      },
      setSpinVariation = {
       args = "(variation: number)",
       description = "Sets the amount of spin variation (0 meaning no variation and 1 meaning full variation between start and end).",
       returns = "()",
       type = "function"
      },
      setSpread = {
       args = "(spread: number)",
       description = "Sets the amount of spread for the system.",
       returns = "()",
       type = "function"
      },
      setTangentialAcceleration = {
       args = "(min: number, max: number)",
       description = "Sets the tangential acceleration (acceleration perpendicular to the particle's direction).",
       returns = "()",
       type = "function"
      },
      setTexture = {
       args = "(texture: Texture)",
       description = "Sets the texture (Image or Canvas) to be used for the particles.",
       returns = "()",
       type = "function"
      },
      start = {
       args = "()",
       description = "Starts the particle emitter.",
       returns = "()",
       type = "function"
      },
      stop = {
       args = "()",
       description = "Stops the particle emitter, resetting the lifetime counter.",
       returns = "()",
       type = "function"
      },
      update = {
       args = "(dt: number)",
       description = "Updates the particle system; moving, creating and killing particles.",
       returns = "()",
       type = "function"
      }
     },
     description = "A ParticleSystem can be used to create particle effects like fire or smoke.\n\nThe particle system has to be created using update it in the update callback to see any changes in the particles emitted.\n\nThe particle system won't create any particles unless you call setParticleLifetime and setEmissionRate.",
     inherits = "love.Drawable, love.Object",
     type = "class"
    },
    Quad = {
     childs = {
      getViewport = {
       args = "()",
       description = "Gets the current viewport of this Quad.",
       returns = "(x: number, y: number, w: number, h: number)",
       type = "function"
      },
      setViewport = {
       args = "(x: number, y: number, w: number, h: number, sw: number, sh: number)",
       description = "Sets the texture coordinates according to a viewport.",
       returns = "()",
       type = "function"
      }
     },
     description = "A quadrilateral (a polygon with four sides and four corners) with texture coordinate information.\n\nQuads can be used to select part of a texture to draw. In this way, one large texture atlas can be loaded, and then split up into sub-images.",
     inherits = "love.Object",
     type = "class"
    },
    Shader = {
     childs = {
      hasUniform = {
       args = "(name: string)",
       description = "Gets whether a uniform / extern variable exists in the Shader.\n\nIf a graphics driver's shader compiler determines that a uniform / extern variable doesn't affect the final output of the shader, it may optimize the variable out. This function will return false in that case.",
       returns = "(hasuniform: boolean)",
       type = "function"
      },
      send = {
       args = "(name: string, number: number, ...: number)",
       description = "Sends one or more values to a special (''uniform'') variable inside the shader. Uniform variables have to be marked using the ''uniform'' or ''extern'' keyword, e.g.\n\nuniform float time;  // 'float' is the typical number type used in GLSL shaders.\n\nuniform float varsvec2 light_pos;\n\nuniform vec4 colors[4;\n\nThe corresponding send calls would be\n\nshader:send('time', t)\n\nshader:send('vars',a,b)\n\nshader:send('light_pos', {light_x, light_y})\n\nshader:send('colors', {r1, g1, b1, a1},  {r2, g2, b2, a2},  {r3, g3, b3, a3},  {r4, g4, b4, a4})\n\nUniform / extern variables are read-only in the shader code and remain constant until modified by a Shader:send call. Uniform variables can be accessed in both the Vertex and Pixel components of a shader, as long as the variable is declared in each.",
       returns = "()",
       type = "function"
      },
      sendColor = {
       args = "(name: string, color: table, ...: table)",
       description = "Sends one or more colors to a special (''extern'' / ''uniform'') vec3 or vec4 variable inside the shader. The color components must be in the range of 1. The colors are gamma-corrected if global gamma-correction is enabled.\n\nExtern variables must be marked using the ''extern'' keyword, e.g.\n\nextern vec4 Color;\n\nThe corresponding sendColor call would be\n\nshader:sendColor('Color', {r, g, b, a})\n\nExtern variables can be accessed in both the Vertex and Pixel stages of a shader, as long as the variable is declared in each.\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.",
       returns = "()",
       type = "function"
      }
     },
     description = "A Shader is used for advanced hardware-accelerated pixel or vertex manipulation. These effects are written in a language based on GLSL (OpenGL Shading Language) with a few things simplified for easier coding.\n\nPotential uses for shaders include HDR/bloom, motion blur, grayscale/invert/sepia/any kind of color effect, reflection/refraction, distortions, bump mapping, and much more! Here is a collection of basic shaders and good starting point to learn: https://github.com/vrld/moonshine",
     inherits = "love.Object",
     type = "class"
    },
    SpriteBatch = {
     childs = {
      addLayer = {
       args = "(layerindex: number, x: number, y: number, r: number, sx: number, sy: number, ox: number, oy: number, kx: number, ky: number)",
       description = "Adds a sprite to a batch created with an Array Texture.",
       returns = "(spriteindex: number)",
       type = "function"
      },
      attachAttribute = {
       args = "(name: string, mesh: Mesh)",
       description = "Attaches a per-vertex attribute from a Mesh onto this SpriteBatch, for use when drawing. This can be combined with a Shader to augment a SpriteBatch with per-vertex or additional per-sprite information instead of just having per-sprite colors.\n\nEach sprite in a SpriteBatch has 4 vertices in the following order: top-left, bottom-left, top-right, bottom-right. The index returned by SpriteBatch:add (and used by SpriteBatch:set) can used to determine the first vertex of a specific sprite with the formula 1 + 4 * ( id - 1 ).",
       returns = "()",
       type = "function"
      },
      clear = {
       args = "()",
       description = "Removes all sprites from the buffer.",
       returns = "()",
       type = "function"
      },
      flush = {
       args = "()",
       description = "Immediately sends all new and modified sprite data in the batch to the graphics card.\n\nNormally it isn't necessary to call this method as love.graphics.draw(spritebatch, ...) will do it automatically if needed, but explicitly using SpriteBatch:flush gives more control over when the work happens.\n\nIf this method is used, it generally shouldn't be called more than once (at most) between love.graphics.draw(spritebatch, ...) calls.",
       returns = "()",
       type = "function"
      },
      getBufferSize = {
       args = "()",
       description = "Gets the maximum number of sprites the SpriteBatch can hold.",
       returns = "(size: number)",
       type = "function"
      },
      getColor = {
       args = "()",
       description = "Gets the color that will be used for the next add and set operations.\n\nIf no color has been set with SpriteBatch:setColor or the current SpriteBatch color has been cleared, this method will return nil.\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.",
       returns = "(r: number, g: number, b: number, a: number)",
       type = "function"
      },
      getCount = {
       args = "()",
       description = "Gets the number of sprites currently in the SpriteBatch.",
       returns = "(count: number)",
       type = "function"
      },
      getTexture = {
       args = "()",
       description = "Gets the texture (Image or Canvas) used by the SpriteBatch.",
       returns = "(texture: Texture)",
       type = "function",
       valuetype = "love.graphics.Texture"
      },
      set = {
       args = "(spriteindex: number, x: number, y: number, r: number, sx: number, sy: number, ox: number, oy: number, kx: number, ky: number)",
       description = "Changes a sprite in the batch. This requires the sprite index returned by SpriteBatch:add or SpriteBatch:addLayer.",
       returns = "()",
       type = "function"
      },
      setColor = {
       args = "(r: number, g: number, b: number, a: number)",
       description = "Sets the color that will be used for the next add and set operations. Calling the function without arguments will disable all per-sprite colors for the SpriteBatch.\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.\n\nIn version 0.9.2 and older, the global color set with love.graphics.setColor will not work on the SpriteBatch if any of the sprites has its own color.",
       returns = "()",
       type = "function"
      },
      setDrawRange = {
       args = "(start: number, count: number)",
       description = "Restricts the drawn sprites in the SpriteBatch to a subset of the total.",
       returns = "()",
       type = "function"
      },
      setLayer = {
       args = "(spriteindex: number, layerindex: number, x: number, y: number, r: number, sx: number, sy: number, ox: number, oy: number, kx: number, ky: number)",
       description = "Changes a sprite previously added with add or addLayer, in a batch created with an Array Texture.",
       returns = "()",
       type = "function"
      },
      setTexture = {
       args = "(texture: Texture)",
       description = "Sets the texture (Image or Canvas) used for the sprites in the batch, when drawing.",
       returns = "()",
       type = "function"
      }
     },
     description = "Using a single image, draw any number of identical copies of the image using a single call to love.graphics.draw(). This can be used, for example, to draw repeating copies of a single background image with high performance.\n\nA SpriteBatch can be even more useful when the underlying image is a texture atlas (a single image file containing many independent images); by adding Quads to the batch, different sub-images from within the atlas can be drawn.",
     inherits = "love.Drawable, love.Object",
     type = "class"
    },
    SpriteBatchUsage = {
     childs = {
      dynamic = {
       description = "The object's data will change occasionally during its lifetime. ",
       type = "value"
      },
      static = {
       description = "The object will not be modified after initial sprites or vertices are added.",
       type = "value"
      },
      stream = {
       description = "The object data will always change between draws.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    StackType = {
     childs = {
      all = {
       description = "All love.graphics state, including transform state.",
       type = "value"
      },
      transform = {
       description = "The transformation stack (love.graphics.translate, love.graphics.rotate, etc.)",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    StencilAction = {
     childs = {
      decrement = {
       description = "The stencil value of a pixel will be decremented by 1 for each object that touches the pixel. If the stencil value reaches 0 it will stay at 0.",
       type = "value"
      },
      decrementwrap = {
       description = "The stencil value of a pixel will be decremented by 1 for each object that touches the pixel. If the stencil value of 0 is decremented it will be set to 255.",
       type = "value"
      },
      increment = {
       description = "The stencil value of a pixel will be incremented by 1 for each object that touches the pixel. If the stencil value reaches 255 it will stay at 255.",
       type = "value"
      },
      incrementwrap = {
       description = "The stencil value of a pixel will be incremented by 1 for each object that touches the pixel. If a stencil value of 255 is incremented it will be set to 0.",
       type = "value"
      },
      invert = {
       description = "The stencil value of a pixel will be bitwise-inverted for each object that touches the pixel. If a stencil value of 0 is inverted it will become 255.",
       type = "value"
      },
      replace = {
       description = "The stencil value of a pixel will be replaced by the value specified in love.graphics.stencil, if any object touches the pixel.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    Text = {
     childs = {
      addf = {
       args = "(textstring: string, wraplimit: number, align: AlignMode, x: number, y: number, angle: number, sx: number, sy: number, ox: number, oy: number, kx: number, ky: number)",
       description = "Adds additional formatted / colored text to the Text object at the specified position.\n\nThe word wrap limit is applied before any scaling, rotation, and other coordinate transformations. Therefore the amount of text per line stays constant given the same wrap limit, even if the scale arguments change.",
       returns = "(index: number)",
       type = "function"
      },
      clear = {
       args = "()",
       description = "Clears the contents of the Text object.",
       returns = "()",
       type = "function"
      },
      getDimensions = {
       args = "(index: number)",
       description = "Gets the width and height of the text in pixels.",
       returns = "(width: number, height: number)",
       type = "function"
      },
      getFont = {
       args = "()",
       description = "Gets the Font used with the Text object.",
       returns = "(font: Font)",
       type = "function",
       valuetype = "love.graphics.Font"
      },
      getHeight = {
       args = "(index: number)",
       description = "Gets the height of the text in pixels.",
       returns = "( height : number)",
       type = "function"
      },
      getWidth = {
       args = "(index: number)",
       description = "Gets the width of the text in pixels.",
       returns = "(width: number)",
       type = "function"
      },
      set = {
       args = "(textstring: string)",
       description = "Replaces the contents of the Text object with a new unformatted string.",
       returns = "()",
       type = "function"
      },
      setFont = {
       args = "(font: Font)",
       description = "Replaces the Font used with the text.",
       returns = "()",
       type = "function"
      },
      setf = {
       args = "(textstring: string, wraplimit: number, align: AlignMode)",
       description = "Replaces the contents of the Text object with a new formatted string.",
       returns = "()",
       type = "function"
      }
     },
     description = "Drawable text.",
     inherits = "love.Drawable, love.Object",
     type = "class"
    },
    Texture = {
     childs = {
      getDepth = {
       args = "()",
       description = "Gets the depth of a Volume Texture. Returns 1 for 2D, Cubemap, and Array textures.",
       returns = "(depth: number)",
       type = "function"
      },
      getFormat = {
       args = "()",
       description = "Gets the pixel format of the Texture.",
       returns = "(format: PixelFormat)",
       type = "function"
      },
      getLayerCount = {
       args = "()",
       description = "Gets the number of layers / slices in an Array Texture. Returns 1 for 2D, Cubemap, and Volume textures.",
       returns = "(layers: number)",
       type = "function"
      },
      getMipmapCount = {
       args = "()",
       description = "Gets the number of mipmaps contained in the Texture. If the texture was not created with mipmaps, it will return 1.",
       returns = "(mipmaps: number)",
       type = "function"
      },
      getMipmapFilter = {
       args = "()",
       description = "Gets the mipmap filter mode for a Texture. Prior to 11.0 this method only worked on Images.",
       returns = "(mode: FilterMode, sharpness: number)",
       type = "function"
      },
      getPixelHeight = {
       args = "()",
       description = "Gets the height in pixels of the Texture.\n\nDPI scale factor, rather than pixels. Use getHeight for calculations related to drawing the texture (calculating an origin offset, for example), and getPixelHeight only when dealing specifically with pixels, for example when using Canvas:newImageData.",
       returns = "(pixelheight: number)",
       type = "function"
      },
      getPixelWidth = {
       args = "()",
       description = "Gets the width in pixels of the Texture.\n\nDPI scale factor, rather than pixels. Use getWidth for calculations related to drawing the texture (calculating an origin offset, for example), and getPixelWidth only when dealing specifically with pixels, for example when using Canvas:newImageData.",
       returns = "(pixelwidth: number)",
       type = "function"
      },
      getTextureType = {
       args = "()",
       description = "Gets the type of the Texture.",
       returns = "(texturetype: TextureType)",
       type = "function"
      },
      isReadable = {
       args = "()",
       description = "Gets whether the Texture can be drawn and sent to a Shader.\n\nCanvases created with stencil and/or depth PixelFormats are not readable by default, unless readable=true is specified in the settings table passed into love.graphics.newCanvas.\n\nNon-readable Canvases can still be rendered to.",
       returns = "(readable: boolean)",
       type = "function"
      },
      setMipmapFilter = {
       args = "(filtermode: FilterMode, sharpness: number)",
       description = "Sets the mipmap filter mode for a Texture. Prior to 11.0 this method only worked on Images.\n\nMipmapping is useful when drawing a texture at a reduced scale. It can improve performance and reduce aliasing issues.\n\nIn created with the mipmaps flag enabled for the mipmap filter to have any effect. In versions prior to 0.10.0 it's best to call this method directly after creating the image with love.graphics.newImage, to avoid bugs in certain graphics drivers.\n\nDue to hardware restrictions and driver bugs, in versions prior to 0.10.0 images that weren't loaded from a CompressedData must have power-of-two dimensions (64x64, 512x256, etc.) to use mipmaps.",
       returns = "()",
       type = "function"
      }
     },
     description = "Superclass for drawable objects which represent a texture. All Textures can be drawn with Quads. This is an abstract type that can't be created directly.",
     inherits = "love.Drawable, love.Object",
     type = "class"
    },
    TextureType = {
     childs = {
      ["2d"] = {
       description = "Regular 2D texture with width and height.",
       type = "value"
      },
      array = {
       description = "Several same-size 2D textures organized into a single object. Similar to a texture atlas / sprite sheet, but avoids sprite bleeding and other issues.",
       type = "value"
      },
      cube = {
       description = "Cubemap texture with 6 faces. Requires a custom shader (and Shader:send) to use. Sampling from a cube texture in a shader takes a 3D direction vector instead of a texture coordinate.",
       type = "value"
      },
      volume = {
       description = "3D texture with width, height, and depth. Requires a custom shader to use. Volume textures can have texture filtering applied along the 3rd axis.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    Video = {
     childs = {
      getFilter = {
       args = "()",
       description = "Gets the scaling filters used when drawing the Video.",
       returns = "(min: FilterMode, mag: FilterMode, anisotropy: number)",
       type = "function"
      },
      getHeight = {
       args = "()",
       description = "Gets the height of the Video in pixels.",
       returns = "(height: number)",
       type = "function"
      },
      getSource = {
       args = "()",
       description = "Gets the audio Source used for playing back the video's audio. May return nil if the video has no audio, or if Video:setSource is called with a nil argument.",
       returns = "(source: Source)",
       type = "function",
       valuetype = "love.audio.Source"
      },
      getStream = {
       args = "()",
       description = "Gets the VideoStream object used for decoding and controlling the video.",
       returns = "(stream: VideoStream)",
       type = "function",
       valuetype = "love.video.VideoStream"
      },
      getWidth = {
       args = "()",
       description = "Gets the width of the Video in pixels.",
       returns = "(width: number)",
       type = "function"
      },
      isPlaying = {
       args = "()",
       description = "Gets whether the Video is currently playing.",
       returns = "(playing: boolean)",
       type = "function"
      },
      pause = {
       args = "()",
       description = "Pauses the Video.",
       returns = "()",
       type = "function"
      },
      play = {
       args = "()",
       description = "Starts playing the Video. In order for the video to appear onscreen it must be drawn with love.graphics.draw.",
       returns = "()",
       type = "function"
      },
      rewind = {
       args = "()",
       description = "Rewinds the Video to the beginning.",
       returns = "()",
       type = "function"
      },
      seek = {
       args = "(offset: number)",
       description = "Sets the current playback position of the Video.",
       returns = "()",
       type = "function"
      },
      setFilter = {
       args = "(min: FilterMode, mag: FilterMode, anisotropy: number)",
       description = "Sets the scaling filters used when drawing the Video.",
       returns = "()",
       type = "function"
      },
      setSource = {
       args = "(source: Source)",
       description = "Sets the audio Source used for playing back the video's audio. The audio Source also controls playback speed and synchronization.",
       returns = "()",
       type = "function"
      },
      tell = {
       args = "()",
       description = "Gets the current playback position of the Video.",
       returns = "(seconds: number)",
       type = "function"
      }
     },
     description = "A drawable video.",
     inherits = "love.Drawable, love.Object",
     type = "class"
    },
    WrapMode = {
     childs = {
      clamp = {
       description = "Clamp the texture. Appears only once. The area outside the texture's normal range is colored based on the edge pixels of the texture.",
       type = "value"
      },
      clampzero = {
       description = "Clamp the texture. Fills the area outside the texture's normal range with transparent black (or opaque black for textures with no alpha channel.)",
       type = "value"
      },
      mirroredrepeat = {
       description = "Repeat the texture, flipping it each time it repeats. May produce better visual results than the repeat mode when the texture doesn't seamlessly tile.",
       type = "value"
      },
      ["repeat"] = {
       description = "Repeat the texture. Fills the whole available extent.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    arc = {
     args = "(drawmode: DrawMode, x: number, y: number, radius: number, angle1: number, angle2: number, segments: number)",
     description = "Draws a filled or unfilled arc at position (x, y). The arc is drawn from angle1 to angle2 in radians. The segments parameter determines how many segments are used to draw the arc. The more segments, the smoother the edge.",
     returns = "()",
     type = "function"
    },
    captureScreenshot = {
     args = "(filename: string)",
     description = "Creates a screenshot once the current frame is done (after love.draw has finished).\n\nSince this function enqueues a screenshot capture rather than executing it immediately, it can be called from an input callback or love.update and it will still capture all of what's drawn to the screen in that frame.",
     returns = "()",
     type = "function"
    },
    circle = {
     args = "(mode: DrawMode, x: number, y: number, radius: number)",
     description = "Draws a circle.",
     returns = "()",
     type = "function"
    },
    clear = {
     args = "(r: number, g: number, b: number, a: number, clearstencil: boolean, cleardepth: boolean)",
     description = "Clears the screen or active Canvas to the specified color.\n\nThis function is called automatically before love.draw in the default love.run function. See the example in love.run for a typical use of this function.\n\nNote that the scissor area bounds the cleared region.\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.\n\nIn versions prior to background color instead.",
     returns = "()",
     type = "function"
    },
    discard = {
     args = "(discardcolor: boolean, discardstencil: boolean)",
     description = "Discards (trashes) the contents of the screen or active Canvas. This is a performance optimization function with niche use cases.\n\nIf the active Canvas has just been changed and the 'replace' BlendMode is about to be used to draw something which covers the entire screen, calling love.graphics.discard rather than calling love.graphics.clear or doing nothing may improve performance on mobile devices.\n\nOn some desktop systems this function may do nothing.",
     returns = "()",
     type = "function"
    },
    draw = {
     args = "(drawable: Drawable, x: number, y: number, r: number, sx: number, sy: number, ox: number, oy: number, kx: number, ky: number)",
     description = "Draws a Drawable object (an Image, Canvas, SpriteBatch, ParticleSystem, Mesh, Text object, or Video) on the screen with optional rotation, scaling and shearing.\n\nObjects are drawn relative to their local coordinate system. The origin is by default located at the top left corner of Image and Canvas. All scaling, shearing, and rotation arguments transform the object relative to that point. Also, the position of the origin can be specified on the screen coordinate system.\n\nIt's possible to rotate an object about its center by offsetting the origin to the center. Angles must be given in radians for rotation. One can also use a negative scaling factor to flip about its centerline. \n\nNote that the offsets are applied before rotation, scaling, or shearing; scaling and shearing are applied before rotation.\n\nThe right and bottom edges of the object are shifted at an angle defined by the shearing factors.\n\nWhen using the default shader anything drawn with this function will be tinted according to the currently selected color.  Set it to pure white to preserve the object's original colors.",
     returns = "()",
     type = "function"
    },
    drawInstanced = {
     args = "(mesh: Mesh, instancecount: number, x: number, y: number, r: number, sx: number, sy: number, ox: number, oy: number, kx: number, ky: number)",
     description = "Draws many instances of a Mesh with a single draw call, using hardware geometry instancing.\n\nEach instance can have unique properties (positions, colors, etc.) but will not by default unless a custom per-instance vertex attributes or the love_InstanceID GLSL 3 vertex shader variable is used, otherwise they will all render at the same position on top of each other.\n\nInstancing is not supported by some older GPUs that are only capable of using OpenGL ES 2 or OpenGL 2. Use love.graphics.getSupported to check.",
     returns = "()",
     type = "function"
    },
    drawLayer = {
     args = "(texture: Texture, layerindex: number, x: number, y: number, r: number, sx: number, sy: number, ox: number, oy: number, kx: number, ky: number)",
     description = "Draws a layer of an Array Texture.",
     returns = "()",
     type = "function"
    },
    ellipse = {
     args = "(mode: DrawMode, x: number, y: number, radiusx: number, radiusy: number)",
     description = "Draws an ellipse.",
     returns = "()",
     type = "function"
    },
    flushBatch = {
     args = "()",
     description = "Immediately renders any pending automatically batched draws.\n\nLÖVE will call this function internally as needed when most state is changed, so it is not necessary to manually call it.\n\nThe current batch will be automatically flushed by color), as well as Shader:send and methods on Textures which change their state. Using a different Image in consecutive love.graphics.draw calls will also flush the current batch.\n\nSpriteBatches, ParticleSystems, Meshes, and Text objects do their own batching and do not affect automatic batching of other draws, aside from flushing the current batch when they're drawn.",
     returns = "()",
     type = "function"
    },
    getBackgroundColor = {
     args = "()",
     description = "Gets the current background color.\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.",
     returns = "(r: number, g: number, b: number, a: number)",
     type = "function"
    },
    getBlendMode = {
     args = "()",
     description = "Gets the blending mode.",
     returns = "(mode: BlendMode, alphamode: BlendAlphaMode)",
     type = "function"
    },
    getCanvas = {
     args = "()",
     description = "Gets the current target Canvas.",
     returns = "(canvas: Canvas)",
     type = "function",
     valuetype = "love.graphics.Canvas"
    },
    getCanvasFormats = {
     args = "(readable: boolean)",
     description = "Gets the available Canvas formats, and whether each is supported.",
     returns = "(formats: table)",
     type = "function"
    },
    getColor = {
     args = "()",
     description = "Gets the current color.\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.",
     returns = "(r: number, g: number, b: number, a: number)",
     type = "function"
    },
    getColorMask = {
     args = "()",
     description = "Gets the active color components used when drawing. Normally all 4 components are active unless love.graphics.setColorMask has been used.\n\nThe color mask determines whether individual components of the colors of drawn objects will affect the color of the screen. They affect love.graphics.clear and Canvas:clear as well.",
     returns = "(r: boolean, g: boolean, b: boolean, a: boolean)",
     type = "function"
    },
    getDPIScale = {
     args = "()",
     description = "Gets the DPI scale factor of the window.\n\nThe DPI scale factor represents relative pixel density. The pixel density inside the window might be greater (or smaller) than the 'size' of the window. For example on a retina screen in Mac OS X with the highdpi window flag enabled, the window may take up the same physical size as an 800x600 window, but the area inside the window uses 1600x1200 pixels. love.graphics.getDPIScale() would return 2 in that case.\n\nThe love.window.fromPixels and love.window.toPixels functions can also be used to convert between units.\n\nThe highdpi window flag must be enabled to use the full pixel density of a Retina screen on Mac OS X and iOS. The flag currently does nothing on Windows and Linux, and on Android it is effectively always enabled.",
     returns = "(scale: number)",
     type = "function"
    },
    getDefaultFilter = {
     args = "()",
     description = "Returns the default scaling filters used with Images, Canvases, and Fonts.",
     returns = "(min: FilterMode, mag: FilterMode, anisotropy: number)",
     type = "function"
    },
    getDepthMode = {
     args = "()",
     description = "Gets the current depth test mode and whether writing to the depth buffer is enabled.\n\nThis is low-level functionality designed for use with custom vertex shaders and Meshes with custom vertex attributes. No higher level APIs are provided to set the depth of 2D graphics such as shapes, lines, and Images.",
     returns = "(comparemode: CompareMode, write: boolean)",
     type = "function"
    },
    getDimensions = {
     args = "()",
     description = "Gets the width and height in pixels of the window.",
     returns = "(width: number, height: number)",
     type = "function"
    },
    getFont = {
     args = "()",
     description = "Gets the current Font object.",
     returns = "(font: Font)",
     type = "function",
     valuetype = "love.graphics.Font"
    },
    getFrontFaceWinding = {
     args = "()",
     description = "Gets whether triangles with clockwise- or counterclockwise-ordered vertices are considered front-facing.\n\nThis is designed for use in combination with Mesh face culling. Other love.graphics shapes, lines, and sprites are not guaranteed to have a specific winding order to their internal vertices.",
     returns = "(winding: VertexWinding)",
     type = "function"
    },
    getHeight = {
     args = "()",
     description = "Gets the height in pixels of the window.",
     returns = "(height: number)",
     type = "function"
    },
    getImageFormats = {
     args = "()",
     description = "Gets the raw and compressed pixel formats usable for Images, and whether each is supported.",
     returns = "(formats: table)",
     type = "function"
    },
    getLineJoin = {
     args = "()",
     description = "Gets the line join style.",
     returns = "(join: LineJoin)",
     type = "function"
    },
    getLineStyle = {
     args = "()",
     description = "Gets the line style.",
     returns = "(style: LineStyle)",
     type = "function"
    },
    getLineWidth = {
     args = "()",
     description = "Gets the current line width.",
     returns = "(width: number)",
     type = "function"
    },
    getMeshCullMode = {
     args = "()",
     description = "Gets whether back-facing triangles in a Mesh are culled.\n\nMesh face culling is designed for use with low level custom hardware-accelerated 3D rendering via custom vertex attributes on Meshes, custom vertex shaders, and depth testing with a depth buffer.",
     returns = "(mode: CullMode)",
     type = "function"
    },
    getPixelHeight = {
     args = "()",
     description = "Gets the height in pixels of the window.\n\nThe graphics coordinate system and DPI scale factor, rather than raw pixels. Use getHeight for calculations related to drawing to the screen and using the coordinate system (calculating the center of the screen, for example), and getPixelHeight only when dealing specifically with underlying pixels (pixel-related calculations in a pixel Shader, for example).",
     returns = "(pixelheight: number)",
     type = "function"
    },
    getPixelWidth = {
     args = "()",
     description = "Gets the width in pixels of the window.\n\nThe graphics coordinate system and DPI scale factor, rather than raw pixels. Use getWidth for calculations related to drawing to the screen and using the coordinate system (calculating the center of the screen, for example), and getPixelWidth only when dealing specifically with underlying pixels (pixel-related calculations in a pixel Shader, for example).",
     returns = "(pixelwidth: number)",
     type = "function"
    },
    getPointSize = {
     args = "()",
     description = "Gets the point size.",
     returns = "(size: number)",
     type = "function"
    },
    getRendererInfo = {
     args = "()",
     description = "Gets information about the system's video card and drivers.",
     returns = "(name: string, version: string, vendor: string, device: string)",
     type = "function"
    },
    getScissor = {
     args = "()",
     description = "Gets the current scissor box.",
     returns = "(x: number, y: number, width: number, height: number)",
     type = "function"
    },
    getShader = {
     args = "()",
     description = "Gets the current Shader. Returns nil if none is set.",
     returns = "(shader: Shader)",
     type = "function",
     valuetype = "love.graphics.Shader"
    },
    getStackDepth = {
     args = "()",
     description = "Gets the current depth of the transform / state stack (the number of pushes without corresponding pops).",
     returns = "(depth: number)",
     type = "function"
    },
    getStats = {
     args = "(stats: table)",
     description = "Gets performance-related rendering statistics. ",
     returns = "(stats: table)",
     type = "function"
    },
    getStencilTest = {
     args = "()",
     description = "Gets the current stencil test configuration.\n\nWhen stencil testing is enabled, the geometry of everything that is drawn afterward will be clipped / stencilled out based on a comparison between the arguments of this function and the stencil value of each pixel that the geometry touches. The stencil values of pixels are affected via love.graphics.stencil.\n\nEach Canvas has its own per-pixel stencil values.",
     returns = "(comparemode: CompareMode, comparevalue: number)",
     type = "function"
    },
    getSupported = {
     args = "()",
     description = "Gets the optional graphics features and whether they're supported on the system.\n\nSome older or low-end systems don't always support all graphics features.",
     returns = "(features: table)",
     type = "function"
    },
    getSystemLimits = {
     args = "()",
     description = "Gets the system-dependent maximum values for love.graphics features.",
     returns = "(limits: table)",
     type = "function"
    },
    getTextureTypes = {
     args = "()",
     description = "Gets the available texture types, and whether each is supported.",
     returns = "(texturetypes: table)",
     type = "function"
    },
    getWidth = {
     args = "()",
     description = "Gets the width in pixels of the window.",
     returns = "(width: number)",
     type = "function"
    },
    intersectScissor = {
     args = "(x: number, y: number, width: number, height: number)",
     description = "Sets the scissor to the rectangle created by the intersection of the specified rectangle with the existing scissor.  If no scissor is active yet, it behaves like love.graphics.setScissor.\n\nThe scissor limits the drawing area to a specified rectangle. This affects all graphics calls, including love.graphics.clear.\n\nThe dimensions of the scissor is unaffected by graphical transformations (translate, scale, ...).",
     returns = "()",
     type = "function"
    },
    inverseTransformPoint = {
     args = "(screenX: number, screenY: number)",
     description = "Converts the given 2D position from screen-space into global coordinates.\n\nThis effectively applies the reverse of the current graphics transformations to the given position. A similar Transform:inverseTransformPoint method exists for Transform objects.",
     returns = "(globalX: number, globalY: number)",
     type = "function"
    },
    isGammaCorrect = {
     args = "()",
     description = "Gets whether gamma-correct rendering is supported and enabled. It can be enabled by setting t.gammacorrect = true in love.conf.\n\nNot all devices support gamma-correct rendering, in which case it will be automatically disabled and this function will return false. It is supported on desktop systems which have graphics cards that are capable of using OpenGL 3 / DirectX 10, and iOS devices that can use OpenGL ES 3.",
     returns = "(gammacorrect: boolean)",
     type = "function"
    },
    isWireframe = {
     args = "()",
     description = "Gets whether wireframe mode is used when drawing.",
     returns = "(wireframe: boolean)",
     type = "function"
    },
    line = {
     args = "(x1: number, y1: number, x2: number, y2: number, ...: number)",
     description = "Draws lines between points.",
     returns = "()",
     type = "function"
    },
    newArrayImage = {
     args = "(slices: table, settings: table)",
     description = "Creates a new array Image.\n\nAn array image / array texture is a single object which contains multiple 'layers' or 'slices' of 2D sub-images. It can be thought of similarly to a texture atlas or sprite sheet, but it doesn't suffer from the same tile / quad bleeding artifacts that texture atlases do – although every sub-image must have the same dimensions.\n\nA specific layer of an array image can be drawn with love.graphics.drawLayer / SpriteBatch:addLayer, or with the Quad variant of love.graphics.draw and Quad:setLayer, or via a custom Shader.\n\nTo use an array image in a Shader, it must be declared as a ArrayImage or sampler2DArray type (instead of Image or sampler2D). The Texel(ArrayImage image, vec3 texturecoord) shader function must be used to get pixel colors from a slice of the array image. The vec3 argument contains the texture coordinate in the first two components, and the 0-based slice index in the third component.",
     returns = "(image: Image)",
     type = "function",
     valuetype = "love.graphics.Image"
    },
    newCanvas = {
     args = "(width: number, height: number)",
     description = "Creates a new Canvas object for offscreen rendering.",
     returns = "(canvas: Canvas)",
     type = "function",
     valuetype = "love.graphics.Canvas"
    },
    newCubeImage = {
     args = "(filename: string, settings: table)",
     description = "Creates a new cubemap Image.\n\nCubemap images have 6 faces (sides) which represent a cube. They can't be rendered directly, they can only be used in Shader code (and sent to the shader via Shader:send).\n\nTo use a cubemap image in a Shader, it must be declared as a CubeImage or samplerCube type (instead of Image or sampler2D). The Texel(CubeImage image, vec3 direction) shader function must be used to get pixel colors from the cubemap. The vec3 argument is a normalized direction from the center of the cube, rather than explicit texture coordinates.\n\nEach face in a cubemap image must have square dimensions.\n\nFor variants of this function which accept a single image containing multiple cubemap faces, they must be laid out in one of the following forms in the image:\n\n   +y\n\n+z +x -z\n\n   -y\n\n   -x\n\nor:\n\n   +y\n\n-x +z +x -z\n\n   -y\n\nor:\n\n+x\n\n-x\n\n+y\n\n-y\n\n+z\n\n-z\n\nor:\n\n+x -x +y -y +z -z",
     returns = "(image: Image)",
     type = "function",
     valuetype = "love.graphics.Image"
    },
    newFont = {
     args = "(filename: string)",
     description = "Creates a new Font from a TrueType Font or BMFont file. Created fonts are not cached, in that calling this function with the same arguments will always create a new Font object.\n\nAll variants which accept a filename can also accept a Data object instead.",
     returns = "(font: Font)",
     type = "function",
     valuetype = "love.graphics.Font"
    },
    newImage = {
     args = "(filename: string)",
     description = "Creates a new Image from a filepath, FileData, an ImageData, or a CompressedImageData, and optionally generates or specifies mipmaps for the image.",
     returns = "(image: Image)",
     type = "function",
     valuetype = "love.graphics.Image"
    },
    newImageFont = {
     args = "(filename: string, glyphs: string)",
     description = "Creates a new specifically formatted image.\n\nIn versions prior to 0.9.0, LÖVE expects ISO 8859-1 encoding for the glyphs string.",
     returns = "(font: Font)",
     type = "function",
     valuetype = "love.graphics.Font"
    },
    newMesh = {
     args = "(vertices: table, mode: MeshDrawMode, usage: SpriteBatchUsage)",
     description = "Creates a new Mesh.\n\nUse Mesh:setTexture if the Mesh should be textured with an Image or Canvas when it's drawn.\n\nIn versions prior to 11.0, color and byte component values were within the range of 0 to 255 instead of 0 to 1.",
     returns = "(mesh: Mesh)",
     type = "function",
     valuetype = "love.graphics.Mesh"
    },
    newParticleSystem = {
     args = "(image: Image, buffer: number)",
     description = "Creates a new ParticleSystem.",
     returns = "(system: ParticleSystem)",
     type = "function",
     valuetype = "love.graphics.ParticleSystem"
    },
    newQuad = {
     args = "(x: number, y: number, width: number, height: number, sw: number, sh: number)",
     description = "Creates a new Quad.\n\nThe purpose of a Quad is to use a fraction of an image to draw objects, as opposed to drawing entire image. It is most useful for sprite sheets and atlases: in a sprite atlas, multiple sprites reside in same image, quad is used to draw a specific sprite from that image; in animated sprites with all frames residing in the same image, quad is used to draw specific frame from the animation.",
     returns = "(quad: Quad)",
     type = "function",
     valuetype = "love.graphics.Quad"
    },
    newShader = {
     args = "(code: string)",
     description = "Creates a new Shader object for hardware-accelerated vertex and pixel effects. A Shader contains either vertex shader code, pixel shader code, or both.\n\nShaders are small programs which are run on the graphics card when drawing. Vertex shaders are run once for each vertex (for example, an image has 4 vertices - one at each corner. A Mesh might have many more.) Pixel shaders are run once for each pixel on the screen which the drawn object touches. Pixel shader code is executed after all the object's vertices have been processed by the vertex shader.",
     returns = "(shader: Shader)",
     type = "function",
     valuetype = "love.graphics.Shader"
    },
    newSpriteBatch = {
     args = "(image: Image, maxsprites: number)",
     description = "Creates a new SpriteBatch object.",
     returns = "(spriteBatch: SpriteBatch)",
     type = "function",
     valuetype = "love.graphics.SpriteBatch"
    },
    newText = {
     args = "(font: Font, textstring: string)",
     description = "Creates a new drawable Text object.",
     returns = "(text: Text)",
     type = "function",
     valuetype = "love.graphics.Text"
    },
    newVideo = {
     args = "(filename: string)",
     description = "Creates a new drawable Video. Currently only Ogg Theora video files are supported.",
     returns = "(video: Video)",
     type = "function",
     valuetype = "love.graphics.Video"
    },
    newVolumeImage = {
     args = "(layers: table, settings: table)",
     description = "Creates a new volume (3D) Image.\n\nVolume images are 3D textures with width, height, and depth. They can't be rendered directly, they can only be used in Shader code (and sent to the shader via Shader:send).\n\nTo use a volume image in a Shader, it must be declared as a VolumeImage or sampler3D type (instead of Image or sampler2D). The Texel(VolumeImage image, vec3 texcoords) shader function must be used to get pixel colors from the volume image. The vec3 argument is a normalized texture coordinate with the z component representing the depth to sample at (ranging from 1).\n\nVolume images are typically used as lookup tables in shaders for color grading, for example, because sampling using a texture coordinate that is partway in between two pixels can interpolate across all 3 dimensions in the volume image, resulting in a smooth gradient even when a small-sized volume image is used as the lookup table.\n\nArray images are a much better choice than volume images for storing multiple different sprites in a single array image for directly drawing them.",
     returns = "(image: Image)",
     type = "function",
     valuetype = "love.graphics.Image"
    },
    origin = {
     args = "()",
     description = "Resets the current coordinate transformation.\n\nThis function is always used to reverse any previous calls to love.graphics.rotate, love.graphics.scale, love.graphics.shear or love.graphics.translate. It returns the current transformation state to its defaults.",
     returns = "()",
     type = "function"
    },
    points = {
     args = "(x: number, y: number, ...: number)",
     description = "Draws one or more points.",
     returns = "()",
     type = "function"
    },
    polygon = {
     args = "(mode: DrawMode, ...: number)",
     description = "Draw a polygon.\n\nFollowing the mode argument, this function can accept multiple numeric arguments or a single table of numeric arguments. In either case the arguments are interpreted as alternating x and y coordinates of the polygon's vertices.",
     returns = "()",
     type = "function"
    },
    pop = {
     args = "()",
     description = "Pops the current coordinate transformation from the transformation stack.\n\nThis function is always used to reverse a previous push operation. It returns the current transformation state to what it was before the last preceding push.",
     returns = "()",
     type = "function"
    },
    present = {
     args = "()",
     description = "Displays the results of drawing operations on the screen.\n\nThis function is used when writing your own love.run function. It presents all the results of your drawing operations on the screen. See the example in love.run for a typical use of this function.",
     returns = "()",
     type = "function"
    },
    print = {
     args = "(text: string, x: number, y: number, r: number, sx: number, sy: number, ox: number, oy: number, kx: number, ky: number)",
     description = "Draws text on screen. If no Font is set, one will be created and set (once) if needed.\n\nAs of LOVE 0.7.1, when using translation and scaling functions while drawing text, this function assumes the scale occurs first.  If you don't script with this in mind, the text won't be in the right position, or possibly even on screen.\n\nlove.graphics.print and love.graphics.printf both support UTF-8 encoding. You'll also need a proper Font for special characters.\n\nIn versions prior to 11.0, color and byte component values were within the range of 0 to 255 instead of 0 to 1.",
     returns = "()",
     type = "function"
    },
    printf = {
     args = "(text: string, x: number, y: number, limit: number, align: AlignMode, r: number, sx: number, sy: number, ox: number, oy: number, kx: number, ky: number)",
     description = "Draws formatted text, with word wrap and alignment.\n\nSee additional notes in love.graphics.print.\n\nThe word wrap limit is applied before any scaling, rotation, and other coordinate transformations. Therefore the amount of text per line stays constant given the same wrap limit, even if the scale arguments change.\n\nIn version 0.9.2 and earlier, wrapping was implemented by breaking up words by spaces and putting them back together to make sure things fit nicely within the limit provided. However, due to the way this is done, extra spaces between words would end up missing when printed on the screen, and some lines could overflow past the provided wrap limit. In version 0.10.0 and newer this is no longer the case.\n\nIn versions prior to 11.0, color and byte component values were within the range of 0 to 255 instead of 0 to 1.",
     returns = "()",
     type = "function"
    },
    push = {
     args = "(stack: StackType)",
     description = "Copies and pushes the current coordinate transformation to the transformation stack.\n\nThis function is always used to prepare for a corresponding pop operation later. It stores the current coordinate transformation state into the transformation stack and keeps it active. Later changes to the transformation can be undone by using the pop operation, which returns the coordinate transform to the state it was in before calling push.",
     returns = "()",
     type = "function"
    },
    rectangle = {
     args = "(mode: DrawMode, x: number, y: number, width: number, height: number)",
     description = "Draws a rectangle.",
     returns = "()",
     type = "function"
    },
    replaceTransform = {
     args = "(transform: Transform)",
     description = "Replaces the current coordinate transformation with the given Transform object.",
     returns = "()",
     type = "function"
    },
    reset = {
     args = "()",
     description = "Resets the current graphics settings.\n\nCalling reset makes the current drawing color white, the current background color black, disables any active color component masks, disables wireframe mode and resets the current graphics transformation to the origin. It also sets both the point and line drawing modes to smooth and their sizes to 1.0.",
     returns = "()",
     type = "function"
    },
    rotate = {
     args = "(angle: number)",
     description = "Rotates the coordinate system in two dimensions.\n\nCalling this function affects all future drawing operations by rotating the coordinate system around the origin by the given amount of radians. This change lasts until love.draw() exits.",
     returns = "()",
     type = "function"
    },
    scale = {
     args = "(sx: number, sy: number)",
     description = "Scales the coordinate system in two dimensions.\n\nBy default the coordinate system in LÖVE corresponds to the display pixels in horizontal and vertical directions one-to-one, and the x-axis increases towards the right while the y-axis increases downwards. Scaling the coordinate system changes this relation.\n\nAfter scaling by sx and sy, all coordinates are treated as if they were multiplied by sx and sy. Every result of a drawing operation is also correspondingly scaled, so scaling by (2, 2) for example would mean making everything twice as large in both x- and y-directions. Scaling by a negative value flips the coordinate system in the corresponding direction, which also means everything will be drawn flipped or upside down, or both. Scaling by zero is not a useful operation.\n\nScale and translate are not commutative operations, therefore, calling them in different orders will change the outcome.\n\nScaling lasts until love.draw() exits.",
     returns = "()",
     type = "function"
    },
    setBackgroundColor = {
     args = "(red: number, green: number, blue: number, alpha: number)",
     description = "Sets the background color.",
     returns = "()",
     type = "function"
    },
    setBlendMode = {
     args = "(mode: BlendMode)",
     description = "Sets the blending mode.",
     returns = "()",
     type = "function"
    },
    setCanvas = {
     args = "(canvas: Canvas, mipmap: number)",
     description = "Captures drawing operations to a Canvas.",
     returns = "()",
     type = "function"
    },
    setColor = {
     args = "(red: number, green: number, blue: number, alpha: number)",
     description = "Sets the color used for drawing.\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.",
     returns = "()",
     type = "function"
    },
    setColorMask = {
     args = "(red: boolean, green: boolean, blue: boolean, alpha: boolean)",
     description = "Sets the color mask. Enables or disables specific color components when rendering and clearing the screen. For example, if '''red''' is set to '''false''', no further changes will be made to the red component of any pixels.",
     returns = "()",
     type = "function"
    },
    setDefaultFilter = {
     args = "(min: FilterMode, mag: FilterMode, anisotropy: number)",
     description = "Sets the default scaling filters used with Images, Canvases, and Fonts.",
     returns = "()",
     type = "function"
    },
    setDepthMode = {
     args = "(comparemode: CompareMode, write: boolean)",
     description = "Configures depth testing and writing to the depth buffer.\n\nThis is low-level functionality designed for use with custom vertex shaders and Meshes with custom vertex attributes. No higher level APIs are provided to set the depth of 2D graphics such as shapes, lines, and Images.",
     returns = "()",
     type = "function"
    },
    setFont = {
     args = "(font: Font)",
     description = "Set an already-loaded Font as the current font or create and load a new one from the file and size.\n\nIt's recommended that Font objects are created with love.graphics.newFont in the loading stage and then passed to this function in the drawing stage.",
     returns = "()",
     type = "function"
    },
    setFrontFaceWinding = {
     args = "(winding: VertexWinding)",
     description = "Sets whether triangles with clockwise- or counterclockwise-ordered vertices are considered front-facing.\n\nThis is designed for use in combination with Mesh face culling. Other love.graphics shapes, lines, and sprites are not guaranteed to have a specific winding order to their internal vertices.",
     returns = "()",
     type = "function"
    },
    setLineJoin = {
     args = "(join: LineJoin)",
     description = "Sets the line join style. See LineJoin for the possible options.",
     returns = "()",
     type = "function"
    },
    setLineStyle = {
     args = "(style: LineStyle)",
     description = "Sets the line style.",
     returns = "()",
     type = "function"
    },
    setLineWidth = {
     args = "(width: number)",
     description = "Sets the line width.",
     returns = "()",
     type = "function"
    },
    setMeshCullMode = {
     args = "(mode: CullMode)",
     description = "Sets whether back-facing triangles in a Mesh are culled.\n\nThis is designed for use with low level custom hardware-accelerated 3D rendering via custom vertex attributes on Meshes, custom vertex shaders, and depth testing with a depth buffer.\n\nBy default, both front- and back-facing triangles in Meshes are rendered.",
     returns = "()",
     type = "function"
    },
    setNewFont = {
     args = "(size: number)",
     description = "Creates and sets a new Font.",
     returns = "(font: Font)",
     type = "function",
     valuetype = "love.graphics.Font"
    },
    setPointSize = {
     args = "(size: number)",
     description = "Sets the point size.",
     returns = "()",
     type = "function"
    },
    setScissor = {
     args = "(x: number, y: number, width: number, height: number)",
     description = "Sets or disables scissor.\n\nThe scissor limits the drawing area to a specified rectangle. This affects all graphics calls, including love.graphics.clear. \n\nThe dimensions of the scissor is unaffected by graphical transformations (translate, scale, ...).",
     returns = "()",
     type = "function"
    },
    setShader = {
     args = "(shader: Shader)",
     description = "Sets or resets a Shader as the current pixel effect or vertex shaders. All drawing operations until the next ''love.graphics.setShader'' will be drawn using the Shader object specified.",
     returns = "()",
     type = "function"
    },
    setStencilTest = {
     args = "(comparemode: CompareMode, comparevalue: number)",
     description = "Configures or disables stencil testing.\n\nWhen stencil testing is enabled, the geometry of everything that is drawn afterward will be clipped / stencilled out based on a comparison between the arguments of this function and the stencil value of each pixel that the geometry touches. The stencil values of pixels are affected via love.graphics.stencil.",
     returns = "()",
     type = "function"
    },
    setWireframe = {
     args = "(enable: boolean)",
     description = "Sets whether wireframe lines will be used when drawing.",
     returns = "()",
     type = "function"
    },
    shear = {
     args = "(kx: number, ky: number)",
     description = "Shears the coordinate system.",
     returns = "()",
     type = "function"
    },
    stencil = {
     args = "(stencilfunction: function, action: StencilAction, value: number, keepvalues: boolean)",
     description = "Draws geometry as a stencil.\n\nThe geometry drawn by the supplied function sets invisible stencil values of pixels, instead of setting pixel colors. The stencil buffer (which contains those stencil values) can act like a mask / stencil - love.graphics.setStencilTest can be used afterward to determine how further rendering is affected by the stencil values in each pixel.\n\nStencil values are integers within the range of 255.",
     returns = "()",
     type = "function"
    },
    transformPoint = {
     args = "(globalX: number, globalY: number)",
     description = "Converts the given 2D position from global coordinates into screen-space.\n\nThis effectively applies the current graphics transformations to the given position. A similar Transform:transformPoint method exists for Transform objects.",
     returns = "(screenX: number, screenY: number)",
     type = "function"
    },
    translate = {
     args = "(dx: number, dy: number)",
     description = "Translates the coordinate system in two dimensions.\n\nWhen this function is called with two numbers, dx, and dy, all the following drawing operations take effect as if their x and y coordinates were x+dx and y+dy. \n\nScale and translate are not commutative operations, therefore, calling them in different orders will change the outcome.\n\nThis change lasts until love.draw() exits or else a love.graphics.pop reverts to a previous love.graphics.push.\n\nTranslating using whole numbers will prevent tearing/blurring of images and fonts draw after translating.",
     returns = "()",
     type = "function"
    },
    validateShader = {
     args = "(gles: boolean, code: string)",
     description = "Validates shader code. Check if specified shader code does not contain any errors.",
     returns = "(status: boolean, message: string)",
     type = "function"
    }
   },
   description = "The primary responsibility for the love.graphics module is the drawing of lines, shapes, text, Images and other Drawable objects onto the screen. Its secondary responsibilities include loading external files (including Images and Fonts) into memory, creating specialized objects (such as ParticleSystems or Canvases) and managing screen geometry.\n\nLÖVE's coordinate system is rooted in the upper-left corner of the screen, which is at location (0, 0). The x axis is horizontal: larger values are further to the right. The y axis is vertical: larger values are further towards the bottom.\n\nIn many cases, you draw images or shapes in terms of their upper-left corner.\n\nMany of the functions are used to manipulate the graphics coordinate system, which is essentially the way coordinates are mapped to the display. You can change the position, scale, and even rotation in this way.",
   type = "class"
  },
  hasDeprecationOutput = {
   args = "()",
   description = "Gets whether LÖVE displays warnings when using deprecated functionality. It is disabled by default in fused mode, and enabled by default otherwise.\n\nWhen deprecation output is enabled, the first use of a formally deprecated LÖVE API will show a message at the bottom of the screen for a short time, and print the message to the console.",
   returns = "(enabled: boolean)",
   type = "function"
  },
  image = {
   childs = {
    CompressedImageData = {
     childs = {
      getFormat = {
       args = "()",
       description = "Gets the format of the CompressedImageData.",
       returns = "(format: CompressedImageFormat)",
       type = "function"
      },
      getHeight = {
       args = "(level: number)",
       description = "Gets the height of the CompressedImageData.",
       returns = "(height: number)",
       type = "function"
      },
      getMipmapCount = {
       args = "()",
       description = "Gets the number of mipmap levels in the CompressedImageData. The base mipmap level (original image) is included in the count.",
       returns = "(mipmaps: number)",
       type = "function"
      },
      getWidth = {
       args = "(level: number)",
       description = "Gets the width of the CompressedImageData.",
       returns = "(width: number)",
       type = "function"
      }
     },
     description = "Represents compressed image data designed to stay compressed in RAM.\n\nCompressedImageData encompasses standard compressed texture formats such as  DXT1, DXT5, and BC5 / 3Dc.\n\nYou can't draw CompressedImageData directly to the screen. See Image for that.",
     inherits = "love.Data, love.Object",
     type = "class"
    },
    CompressedImageFormat = {
     childs = {
      ASTC4x4 = {
       description = "The 4x4 pixels per block variant of the ASTC format. RGBA data at 8 bits per pixel.",
       type = "value"
      },
      ASTC5x4 = {
       description = "The 5x4 pixels per block variant of the ASTC format. RGBA data at 6.4 bits per pixel.",
       type = "value"
      },
      ASTC5x5 = {
       description = "The 5x5 pixels per block variant of the ASTC format. RGBA data at 5.12 bits per pixel.",
       type = "value"
      },
      ASTC6x5 = {
       description = "The 6x5 pixels per block variant of the ASTC format. RGBA data at 4.27 bits per pixel.",
       type = "value"
      },
      ASTC6x6 = {
       description = "The 6x6 pixels per block variant of the ASTC format. RGBA data at 3.56 bits per pixel.",
       type = "value"
      },
      ASTC8x5 = {
       description = "The 8x5 pixels per block variant of the ASTC format. RGBA data at 3.2 bits per pixel.",
       type = "value"
      },
      ASTC8x6 = {
       description = "The 8x6 pixels per block variant of the ASTC format. RGBA data at 2.67 bits per pixel.",
       type = "value"
      },
      ASTC8x8 = {
       description = "The 8x8 pixels per block variant of the ASTC format. RGBA data at 2 bits per pixel.",
       type = "value"
      },
      ASTC10x5 = {
       description = "The 10x5 pixels per block variant of the ASTC format. RGBA data at 2.56 bits per pixel.",
       type = "value"
      },
      ASTC10x6 = {
       description = "The 10x6 pixels per block variant of the ASTC format. RGBA data at 2.13 bits per pixel.",
       type = "value"
      },
      ASTC10x8 = {
       description = "The 10x8 pixels per block variant of the ASTC format. RGBA data at 1.6 bits per pixel.",
       type = "value"
      },
      ASTC10x10 = {
       description = "The 10x10 pixels per block variant of the ASTC format. RGBA data at 1.28 bits per pixel.",
       type = "value"
      },
      ASTC12x10 = {
       description = "The 12x10 pixels per block variant of the ASTC format. RGBA data at 1.07 bits per pixel.",
       type = "value"
      },
      ASTC12x12 = {
       description = "The 12x12 pixels per block variant of the ASTC format. RGBA data at 0.89 bits per pixel.",
       type = "value"
      },
      BC4 = {
       description = "The BC4 format (also known as 3Dc+ or ATI1.) Stores just the red channel, at 4 bits per pixel.",
       type = "value"
      },
      BC4s = {
       description = "The signed variant of the BC4 format. Same as above but pixel values in the texture are in the range of 1 instead of 1 in shaders.",
       type = "value"
      },
      BC5 = {
       description = "The BC5 format (also known as 3Dc or ATI2.) Stores red and green channels at 8 bits per pixel.",
       type = "value"
      },
      BC5s = {
       description = "The signed variant of the BC5 format.",
       type = "value"
      },
      BC6h = {
       description = "The BC6H format. Stores half-precision floating-point RGB data in the range of 65504 at 8 bits per pixel. Suitable for HDR images on desktop systems.",
       type = "value"
      },
      BC6hs = {
       description = "The signed variant of the BC6H format. Stores RGB data in the range of +65504.",
       type = "value"
      },
      BC7 = {
       description = "The BC7 format (also known as BPTC.) Stores RGB or RGBA data at 8 bits per pixel.",
       type = "value"
      },
      DXT1 = {
       description = "The DXT1 format. RGB data at 4 bits per pixel (compared to 32 bits for ImageData and regular Images.) Suitable for fully opaque images on desktop systems.",
       type = "value"
      },
      DXT3 = {
       description = "The DXT3 format. RGBA data at 8 bits per pixel. Smooth variations in opacity do not mix well with this format.",
       type = "value"
      },
      DXT5 = {
       description = "The DXT5 format. RGBA data at 8 bits per pixel. Recommended for images with varying opacity on desktop systems.",
       type = "value"
      },
      EACr = {
       description = "The single-channel variant of the EAC format. Stores just the red channel, at 4 bits per pixel.",
       type = "value"
      },
      EACrg = {
       description = "The two-channel variant of the EAC format. Stores red and green channels at 8 bits per pixel.",
       type = "value"
      },
      EACrgs = {
       description = "The signed two-channel variant of the EAC format.",
       type = "value"
      },
      EACrs = {
       description = "The signed single-channel variant of the EAC format. Same as above but pixel values in the texture are in the range of 1 instead of 1 in shaders.",
       type = "value"
      },
      ETC1 = {
       description = "The ETC1 format. RGB data at 4 bits per pixel. Suitable for fully opaque images on older Android devices.",
       type = "value"
      },
      ETC2rgb = {
       description = "The RGB variant of the ETC2 format. RGB data at 4 bits per pixel. Suitable for fully opaque images on newer mobile devices.",
       type = "value"
      },
      ETC2rgba = {
       description = "The RGBA variant of the ETC2 format. RGBA data at 8 bits per pixel. Recommended for images with varying opacity on newer mobile devices.",
       type = "value"
      },
      ETC2rgba1 = {
       description = "The RGBA variant of the ETC2 format where pixels are either fully transparent or fully opaque. RGBA data at 4 bits per pixel.",
       type = "value"
      },
      PVR1rgb2 = {
       description = "The 2 bit per pixel RGB variant of the PVRTC1 format. Stores RGB data at 2 bits per pixel. Textures compressed with PVRTC1 formats must be square and power-of-two sized.",
       type = "value"
      },
      PVR1rgb4 = {
       description = "The 4 bit per pixel RGB variant of the PVRTC1 format. Stores RGB data at 4 bits per pixel.",
       type = "value"
      },
      PVR1rgba2 = {
       description = "The 2 bit per pixel RGBA variant of the PVRTC1 format.",
       type = "value"
      },
      PVR1rgba4 = {
       description = "The 4 bit per pixel RGBA variant of the PVRTC1 format.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    ImageData = {
     childs = {
      getDimensions = {
       args = "()",
       description = "Gets the width and height of the ImageData in pixels.",
       returns = "(width: number, height: number)",
       type = "function"
      },
      getHeight = {
       args = "()",
       description = "Gets the height of the ImageData in pixels.",
       returns = "(height: number)",
       type = "function"
      },
      getPixel = {
       args = "(x: number, y: number)",
       description = "Gets the color of a pixel at a specific position in the image.\n\nValid x and y values start at 0 and go up to image width and height minus 1. Non-integer values are floored.\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.",
       returns = "(r: number, g: number, b: number, a: number)",
       type = "function"
      },
      getWidth = {
       args = "()",
       description = "Gets the width of the ImageData in pixels.",
       returns = "(width: number)",
       type = "function"
      },
      mapPixel = {
       args = "(pixelFunction: function, x: number, y: number, width: number, height: number)",
       description = "Transform an image by applying a function to every pixel.\n\nThis function is a higher-order function. It takes another function as a parameter, and calls it once for each pixel in the ImageData.\n\nThe passed function is called with six parameters for each pixel in turn. The parameters are numbers that represent the x and y coordinates of the pixel and its red, green, blue and alpha values. The function should return the new red, green, blue, and alpha values for that pixel.\n\nfunction pixelFunction(x, y, r, g, b, a)\n\n    -- template for defining your own pixel mapping function\n\n    -- perform computations giving the new values for r, g, b and a\n\n    -- ...\n\n    return r, g, b, a\n\nend\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.",
       returns = "()",
       type = "function"
      },
      paste = {
       args = "(source: ImageData, dx: number, dy: number, sx: number, sy: number, sw: number, sh: number)",
       description = "Paste into ImageData from another source ImageData.",
       returns = "()",
       type = "function"
      },
      setPixel = {
       args = "(x: number, y: number, r: number, g: number, b: number, a: number)",
       description = "Sets the color of a pixel at a specific position in the image.\n\nValid x and y values start at 0 and go up to image width and height minus 1.\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.",
       returns = "()",
       type = "function"
      }
     },
     description = "Raw (decoded) image data.\n\nYou can't draw ImageData directly to screen. See Image for that.",
     inherits = "love.Data, love.Object",
     type = "class"
    },
    ImageFormat = {
     childs = {
      bmp = {
       description = "BMP image format.",
       type = "value"
      },
      jpg = {
       description = "JPG image format.",
       type = "value"
      },
      png = {
       description = "PNG image format.",
       type = "value"
      },
      tga = {
       description = "Targa image format.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    newCompressedData = {
     args = "(filename: string)",
     description = "Create a new CompressedImageData object from a compressed image file. LÖVE supports several compressed texture formats, enumerated in the CompressedImageFormat page.",
     returns = "(compressedImageData: CompressedImageData)",
     type = "function",
     valuetype = "love.image.CompressedImageData"
    },
    newImageData = {
     args = "(width: number, height: number)",
     description = "Creates a new ImageData object.",
     returns = "(imageData: ImageData)",
     type = "function",
     valuetype = "love.image.ImageData"
    }
   },
   description = "Provides an interface to decode encoded image data.",
   type = "class"
  },
  joystick = {
   childs = {
    GamepadAxis = {
     childs = {
      leftx = {
       description = "The x-axis of the left thumbstick.",
       type = "value"
      },
      lefty = {
       description = "The y-axis of the left thumbstick.",
       type = "value"
      },
      rightx = {
       description = "The x-axis of the right thumbstick.",
       type = "value"
      },
      righty = {
       description = "The y-axis of the right thumbstick.",
       type = "value"
      },
      triggerleft = {
       description = "Left analog trigger.",
       type = "value"
      },
      triggerright = {
       description = "Right analog trigger.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    GamepadButton = {
     childs = {
      a = {
       description = "Bottom face button (A).",
       type = "value"
      },
      b = {
       description = "Right face button (B).",
       type = "value"
      },
      back = {
       description = "Back button.",
       type = "value"
      },
      dpdown = {
       description = "D-pad down.",
       type = "value"
      },
      dpleft = {
       description = "D-pad left.",
       type = "value"
      },
      dpright = {
       description = "D-pad right.",
       type = "value"
      },
      dpup = {
       description = "D-pad up.",
       type = "value"
      },
      guide = {
       description = "Guide button.",
       type = "value"
      },
      leftshoulder = {
       description = "Left bumper.",
       type = "value"
      },
      leftstick = {
       description = "Left stick click button.",
       type = "value"
      },
      rightshoulder = {
       description = "Right bumper.",
       type = "value"
      },
      rightstick = {
       description = "Right stick click button.",
       type = "value"
      },
      start = {
       description = "Start button.",
       type = "value"
      },
      x = {
       description = "Left face button (X).",
       type = "value"
      },
      y = {
       description = "Top face button (Y).",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    Joystick = {
     childs = {
      getAxis = {
       args = "(axis: number)",
       description = "Gets the direction of an axis.",
       returns = "(direction: number)",
       type = "function"
      },
      getAxisCount = {
       args = "()",
       description = "Gets the number of axes on the joystick.",
       returns = "(axes: number)",
       type = "function"
      },
      getButtonCount = {
       args = "()",
       description = "Gets the number of buttons on the joystick.",
       returns = "(buttons: number)",
       type = "function"
      },
      getDeviceInfo = {
       args = "()",
       description = "Gets the USB vendor ID, product ID, and product version numbers of joystick which consistent across operating systems.\n\nCan be used to show different icons, etc. for different gamepads.",
       returns = "(vendorID: number, productID: number, productVersion: number)",
       type = "function"
      },
      getGUID = {
       args = "()",
       description = "Gets a stable GUID unique to the type of the physical joystick which does not change over time. For example, all Sony Dualshock 3 controllers in OS X have the same GUID. The value is platform-dependent.",
       returns = "(guid: string)",
       type = "function"
      },
      getGamepadAxis = {
       args = "(axis: GamepadAxis)",
       description = "Gets the direction of a virtual gamepad axis. If the Joystick isn't recognized as a gamepad or isn't connected, this function will always return 0.",
       returns = "(direction: number)",
       type = "function"
      },
      getGamepadMapping = {
       args = "(axis: GamepadAxis)",
       description = "Gets the button, axis or hat that a virtual gamepad input is bound to.",
       returns = "(inputtype: JoystickInputType, inputindex: number, hatdirection: JoystickHat)",
       type = "function"
      },
      getGamepadMappingString = {
       args = "()",
       description = "Gets the full gamepad mapping string of this Joystick, or nil if it's not recognized as a gamepad.\n\nThe mapping string contains binding information used to map the Joystick's buttons an axes to the standard gamepad layout, and can be used later with love.joystick.loadGamepadMappings.",
       returns = "(mappingstring: string)",
       type = "function"
      },
      getHat = {
       args = "(hat: number)",
       description = "Gets the direction of the Joystick's hat.",
       returns = "(direction: JoystickHat)",
       type = "function"
      },
      getHatCount = {
       args = "()",
       description = "Gets the number of hats on the joystick.",
       returns = "(hats: number)",
       type = "function"
      },
      getID = {
       args = "()",
       description = "Gets the joystick's unique identifier. The identifier will remain the same for the life of the game, even when the Joystick is disconnected and reconnected, but it '''will''' change when the game is re-launched.",
       returns = "(id: number, instanceid: number)",
       type = "function"
      },
      getName = {
       args = "()",
       description = "Gets the name of the joystick.",
       returns = "(name: string)",
       type = "function"
      },
      getVibration = {
       args = "()",
       description = "Gets the current vibration motor strengths on a Joystick with rumble support.",
       returns = "(left: number, right: number)",
       type = "function"
      },
      isConnected = {
       args = "()",
       description = "Gets whether the Joystick is connected.",
       returns = "(connected: boolean)",
       type = "function"
      },
      isDown = {
       args = "(buttonN: number)",
       description = "Checks if a button on the Joystick is pressed.\n\nLÖVE 0.9.0 had a bug which required the button indices passed to Joystick:isDown to be 0-based instead of 1-based, for example button 1 would be 0 for this function. It was fixed in 0.9.1.",
       returns = "(anyDown: boolean)",
       type = "function"
      },
      isGamepad = {
       args = "()",
       description = "Gets whether the Joystick is recognized as a gamepad. If this is the case, the Joystick's buttons and axes can be used in a standardized manner across different operating systems and joystick models via Joystick:getGamepadAxis, Joystick:isGamepadDown, love.gamepadpressed, and related functions.\n\nLÖVE automatically recognizes most popular controllers with a similar layout to the Xbox 360 controller as gamepads, but you can add more with love.joystick.setGamepadMapping.",
       returns = "(isgamepad: boolean)",
       type = "function"
      },
      isGamepadDown = {
       args = "(buttonN: GamepadButton)",
       description = "Checks if a virtual gamepad button on the Joystick is pressed. If the Joystick is not recognized as a Gamepad or isn't connected, then this function will always return false.",
       returns = "(anyDown: boolean)",
       type = "function"
      },
      isVibrationSupported = {
       args = "()",
       description = "Gets whether the Joystick supports vibration.",
       returns = "(supported: boolean)",
       type = "function"
      },
      setVibration = {
       args = "(left: number, right: number)",
       description = "Sets the vibration motor speeds on a Joystick with rumble support. Most common gamepads have this functionality, although not all drivers give proper support. Use Joystick:isVibrationSupported to check.",
       returns = "(success: boolean)",
       type = "function"
      }
     },
     description = "Represents a physical joystick.",
     inherits = "love.Object",
     type = "class"
    },
    JoystickHat = {
     childs = {
      c = {
       description = "Centered",
       type = "value"
      },
      d = {
       description = "Down",
       type = "value"
      },
      l = {
       description = "Left",
       type = "value"
      },
      ld = {
       description = "Left+Down",
       type = "value"
      },
      lu = {
       description = "Left+Up",
       type = "value"
      },
      r = {
       description = "Right",
       type = "value"
      },
      rd = {
       description = "Right+Down",
       type = "value"
      },
      ru = {
       description = "Right+Up",
       type = "value"
      },
      u = {
       description = "Up",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    JoystickInputType = {
     childs = {
      axis = {
       description = "Analog axis.",
       type = "value"
      },
      button = {
       description = "Button.",
       type = "value"
      },
      hat = {
       description = "8-direction hat value.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    getJoystickCount = {
     args = "()",
     description = "Gets the number of connected joysticks.",
     returns = "(joystickcount: number)",
     type = "function"
    },
    getJoysticks = {
     args = "()",
     description = "Gets a list of connected Joysticks.",
     returns = "(joysticks: table)",
     type = "function"
    },
    loadGamepadMappings = {
     args = "(filename: string)",
     description = "Loads a gamepad mappings string or file created with love.joystick.saveGamepadMappings.\n\nIt also recognizes any SDL gamecontroller mapping string, such as those created with Steam's Big Picture controller configure interface, or this nice database. If a new mapping is loaded for an already known controller GUID, the later version will overwrite the one currently loaded.",
     returns = "()",
     type = "function"
    },
    saveGamepadMappings = {
     args = "(filename: string)",
     description = "Saves the virtual gamepad mappings of all recognized as gamepads and have either been recently used or their gamepad bindings have been modified.\n\nThe mappings are stored as a string for use with love.joystick.loadGamepadMappings.",
     returns = "(mappings: string)",
     type = "function"
    },
    setGamepadMapping = {
     args = "(guid: string, button: GamepadButton, inputtype: JoystickInputType, inputindex: number, hatdir: JoystickHat)",
     description = "Binds a virtual gamepad input to a button, axis or hat for all Joysticks of a certain type. For example, if this function is used with a GUID returned by a Dualshock 3 controller in OS X, the binding will affect Joystick:getGamepadAxis and Joystick:isGamepadDown for ''all'' Dualshock 3 controllers used with the game when run in OS X.\n\nLÖVE includes built-in gamepad bindings for many common controllers. This function lets you change the bindings or add new ones for types of Joysticks which aren't recognized as gamepads by default.\n\nThe virtual gamepad buttons and axes are designed around the Xbox 360 controller layout.",
     returns = "(success: boolean)",
     type = "function"
    }
   },
   description = "Provides an interface to the user's joystick.",
   type = "class"
  },
  joystickadded = {
   args = "(joystick: Joystick)",
   description = "Called when a Joystick is connected.",
   returns = "()",
   type = "function"
  },
  joystickaxis = {
   args = "(joystick: Joystick, axis: number, value: number)",
   description = "Called when a joystick axis moves.",
   returns = "()",
   type = "function"
  },
  joystickhat = {
   args = "(joystick: Joystick, hat: number, direction: JoystickHat)",
   description = "Called when a joystick hat direction changes.",
   returns = "()",
   type = "function"
  },
  joystickpressed = {
   args = "(joystick: Joystick, button: number)",
   description = "Called when a joystick button is pressed.",
   returns = "()",
   type = "function"
  },
  joystickreleased = {
   args = "(joystick: Joystick, button: number)",
   description = "Called when a joystick button is released.",
   returns = "()",
   type = "function"
  },
  joystickremoved = {
   args = "(joystick: Joystick)",
   description = "Called when a Joystick is disconnected.",
   returns = "()",
   type = "function"
  },
  keyboard = {
   childs = {
    KeyConstant = {
     childs = {
      ["!"] = {
       description = "Exclamation mark key",
       type = "value"
      },
      ["\""] = {
       description = "Double quote key",
       type = "value"
      },
      ["#"] = {
       description = "Hash key",
       type = "value"
      },
      ["$"] = {
       description = "Dollar key",
       type = "value"
      },
      ["&"] = {
       description = "Ampersand key",
       type = "value"
      },
      ["'"] = {
       description = "Single quote key",
       type = "value"
      },
      ["("] = {
       description = "Left parenthesis key",
       type = "value"
      },
      [")"] = {
       description = "Right parenthesis key",
       type = "value"
      },
      ["*"] = {
       description = "Asterisk key",
       type = "value"
      },
      ["+"] = {
       description = "Plus key",
       type = "value"
      },
      [","] = {
       description = "Comma key",
       type = "value"
      },
      ["-"] = {
       description = "Hyphen-minus key",
       type = "value"
      },
      ["."] = {
       description = "Full stop key",
       type = "value"
      },
      ["/"] = {
       description = "Slash key",
       type = "value"
      },
      ["0"] = {
       description = "The zero key",
       type = "value"
      },
      ["1"] = {
       description = "The one key",
       type = "value"
      },
      ["2"] = {
       description = "The two key",
       type = "value"
      },
      ["3"] = {
       description = "The three key",
       type = "value"
      },
      ["4"] = {
       description = "The four key",
       type = "value"
      },
      ["5"] = {
       description = "The five key",
       type = "value"
      },
      ["6"] = {
       description = "The six key",
       type = "value"
      },
      ["7"] = {
       description = "The seven key",
       type = "value"
      },
      ["8"] = {
       description = "The eight key",
       type = "value"
      },
      ["9"] = {
       description = "The nine key",
       type = "value"
      },
      [":"] = {
       description = "Colon key",
       type = "value"
      },
      [";"] = {
       description = "Semicolon key",
       type = "value"
      },
      ["<"] = {
       description = "Less-than key",
       type = "value"
      },
      ["="] = {
       description = "Equal key",
       type = "value"
      },
      [">"] = {
       description = "Greater-than key",
       type = "value"
      },
      ["?"] = {
       description = "Question mark key",
       type = "value"
      },
      ["@"] = {
       description = "At sign key",
       type = "value"
      },
      ["["] = {
       description = "Left square bracket key",
       type = "value"
      },
      ["\\"] = {
       description = "Backslash key",
       type = "value"
      },
      ["]"] = {
       description = "Right square bracket key",
       type = "value"
      },
      ["^"] = {
       description = "Caret key",
       type = "value"
      },
      _ = {
       description = "Underscore key",
       type = "value"
      },
      ["`"] = {
       description = "Grave accent key",
       type = "value"
      },
      a = {
       description = "The A key",
       type = "value"
      },
      appback = {
       description = "Application back key",
       type = "value"
      },
      appbookmarks = {
       description = "Application bookmarks key",
       type = "value"
      },
      appforward = {
       description = "Application forward key",
       type = "value"
      },
      apphome = {
       description = "Application home key",
       type = "value"
      },
      apprefresh = {
       description = "Application refresh key",
       type = "value"
      },
      appsearch = {
       description = "Application search key",
       type = "value"
      },
      b = {
       description = "The B key",
       type = "value"
      },
      backspace = {
       description = "Backspace key",
       type = "value"
      },
      ["break"] = {
       description = "Break key",
       type = "value"
      },
      c = {
       description = "The C key",
       type = "value"
      },
      calculator = {
       description = "Calculator key",
       type = "value"
      },
      capslock = {
       description = "Caps-lock key",
       type = "value"
      },
      clear = {
       description = "Clear key",
       type = "value"
      },
      compose = {
       description = "Compose key",
       type = "value"
      },
      d = {
       description = "The D key",
       type = "value"
      },
      delete = {
       description = "Delete key",
       type = "value"
      },
      down = {
       description = "Down cursor key",
       type = "value"
      },
      e = {
       description = "The E key",
       type = "value"
      },
      ["end"] = {
       description = "End key",
       type = "value"
      },
      escape = {
       description = "Escape key",
       type = "value"
      },
      euro = {
       description = "Euro (&euro;) key",
       type = "value"
      },
      f = {
       description = "The F key",
       type = "value"
      },
      f1 = {
       description = "The 1st function key",
       type = "value"
      },
      f2 = {
       description = "The 2nd function key",
       type = "value"
      },
      f3 = {
       description = "The 3rd function key",
       type = "value"
      },
      f4 = {
       description = "The 4th function key",
       type = "value"
      },
      f5 = {
       description = "The 5th function key",
       type = "value"
      },
      f6 = {
       description = "The 6th function key",
       type = "value"
      },
      f7 = {
       description = "The 7th function key",
       type = "value"
      },
      f8 = {
       description = "The 8th function key",
       type = "value"
      },
      f9 = {
       description = "The 9th function key",
       type = "value"
      },
      f10 = {
       description = "The 10th function key",
       type = "value"
      },
      f11 = {
       description = "The 11th function key",
       type = "value"
      },
      f12 = {
       description = "The 12th function key",
       type = "value"
      },
      f13 = {
       description = "The 13th function key",
       type = "value"
      },
      f14 = {
       description = "The 14th function key",
       type = "value"
      },
      f15 = {
       description = "The 15th function key",
       type = "value"
      },
      g = {
       description = "The G key",
       type = "value"
      },
      h = {
       description = "The H key",
       type = "value"
      },
      help = {
       description = "Help key",
       type = "value"
      },
      home = {
       description = "Home key",
       type = "value"
      },
      i = {
       description = "The I key",
       type = "value"
      },
      insert = {
       description = "Insert key",
       type = "value"
      },
      j = {
       description = "The J key",
       type = "value"
      },
      k = {
       description = "The K key",
       type = "value"
      },
      ["kp*"] = {
       description = "The numpad multiplication key",
       type = "value"
      },
      ["kp+"] = {
       description = "The numpad addition key",
       type = "value"
      },
      ["kp-"] = {
       description = "The numpad substraction key",
       type = "value"
      },
      ["kp."] = {
       description = "The numpad decimal point key",
       type = "value"
      },
      ["kp/"] = {
       description = "The numpad division key",
       type = "value"
      },
      kp0 = {
       description = "The numpad zero key",
       type = "value"
      },
      kp1 = {
       description = "The numpad one key",
       type = "value"
      },
      kp2 = {
       description = "The numpad two key",
       type = "value"
      },
      kp3 = {
       description = "The numpad three key",
       type = "value"
      },
      kp4 = {
       description = "The numpad four key",
       type = "value"
      },
      kp5 = {
       description = "The numpad five key",
       type = "value"
      },
      kp6 = {
       description = "The numpad six key",
       type = "value"
      },
      kp7 = {
       description = "The numpad seven key",
       type = "value"
      },
      kp8 = {
       description = "The numpad eight key",
       type = "value"
      },
      kp9 = {
       description = "The numpad nine key",
       type = "value"
      },
      ["kp="] = {
       description = "The numpad equals key",
       type = "value"
      },
      kpenter = {
       description = "The numpad enter key",
       type = "value"
      },
      l = {
       description = "The L key",
       type = "value"
      },
      lalt = {
       description = "Left alt key",
       type = "value"
      },
      lctrl = {
       description = "Left control key",
       type = "value"
      },
      left = {
       description = "Left cursor key",
       type = "value"
      },
      lmeta = {
       description = "Left meta key",
       type = "value"
      },
      lshift = {
       description = "Left shift key",
       type = "value"
      },
      lsuper = {
       description = "Left super key",
       type = "value"
      },
      m = {
       description = "The M key",
       type = "value"
      },
      mail = {
       description = "Mail key",
       type = "value"
      },
      menu = {
       description = "Menu key",
       type = "value"
      },
      mode = {
       description = "Mode key",
       type = "value"
      },
      n = {
       description = "The N key",
       type = "value"
      },
      numlock = {
       description = "Num-lock key",
       type = "value"
      },
      o = {
       description = "The O key",
       type = "value"
      },
      p = {
       description = "The P key",
       type = "value"
      },
      pagedown = {
       description = "Page down key",
       type = "value"
      },
      pageup = {
       description = "Page up key",
       type = "value"
      },
      pause = {
       description = "Pause key",
       type = "value"
      },
      power = {
       description = "Power key",
       type = "value"
      },
      print = {
       description = "Print key",
       type = "value"
      },
      q = {
       description = "The Q key",
       type = "value"
      },
      r = {
       description = "The R key",
       type = "value"
      },
      ralt = {
       description = "Right alt key",
       type = "value"
      },
      rctrl = {
       description = "Right control key",
       type = "value"
      },
      ["return"] = {
       description = "Return key",
       type = "value"
      },
      right = {
       description = "Right cursor key",
       type = "value"
      },
      rmeta = {
       description = "Right meta key",
       type = "value"
      },
      rshift = {
       description = "Right shift key",
       type = "value"
      },
      rsuper = {
       description = "Right super key",
       type = "value"
      },
      s = {
       description = "The S key",
       type = "value"
      },
      scrollock = {
       description = "Scroll-lock key",
       type = "value"
      },
      space = {
       description = "Space key",
       type = "value"
      },
      sysreq = {
       description = "System request key",
       type = "value"
      },
      t = {
       description = "The T key",
       type = "value"
      },
      tab = {
       description = "Tab key",
       type = "value"
      },
      u = {
       description = "The U key",
       type = "value"
      },
      undo = {
       description = "Undo key",
       type = "value"
      },
      up = {
       description = "Up cursor key",
       type = "value"
      },
      v = {
       description = "The V key",
       type = "value"
      },
      w = {
       description = "The W key",
       type = "value"
      },
      www = {
       description = "WWW key",
       type = "value"
      },
      x = {
       description = "The X key",
       type = "value"
      },
      y = {
       description = "The Y key",
       type = "value"
      },
      z = {
       description = "The Z key",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    Scancode = {
     childs = {
      ["'"] = {
       description = "The apostrophe key on an American layout.",
       type = "value"
      },
      [","] = {
       description = "The comma key on an American layout.",
       type = "value"
      },
      ["-"] = {
       description = "The minus key on an American layout.",
       type = "value"
      },
      ["."] = {
       description = "The period key on an American layout.",
       type = "value"
      },
      ["/"] = {
       description = "The forward-slash key on an American layout.",
       type = "value"
      },
      ["0"] = {
       description = "The '0' key on an American layout.",
       type = "value"
      },
      ["1"] = {
       description = "The '1' key on an American layout.",
       type = "value"
      },
      ["2"] = {
       description = "The '2' key on an American layout.",
       type = "value"
      },
      ["3"] = {
       description = "The '3' key on an American layout.",
       type = "value"
      },
      ["4"] = {
       description = "The '4' key on an American layout.",
       type = "value"
      },
      ["5"] = {
       description = "The '5' key on an American layout.",
       type = "value"
      },
      ["6"] = {
       description = "The '6' key on an American layout.",
       type = "value"
      },
      ["7"] = {
       description = "The '7' key on an American layout.",
       type = "value"
      },
      ["8"] = {
       description = "The '8' key on an American layout.",
       type = "value"
      },
      ["9"] = {
       description = "The '9' key on an American layout.",
       type = "value"
      },
      [";"] = {
       description = "The semicolon key on an American layout.",
       type = "value"
      },
      ["="] = {
       description = "The equals key on an American layout.",
       type = "value"
      },
      ["["] = {
       description = "The left-bracket key on an American layout.",
       type = "value"
      },
      ["\\"] = {
       description = "The backslash key on an American layout.",
       type = "value"
      },
      ["]"] = {
       description = "The right-bracket key on an American layout.",
       type = "value"
      },
      ["`"] = {
       description = "The back-tick / grave key on an American layout.",
       type = "value"
      },
      a = {
       description = "The 'A' key on an American layout.",
       type = "value"
      },
      acback = {
       description = "The AC Back key on an American layout.",
       type = "value"
      },
      acbookmarks = {
       description = "The AC Bookmarks key on an American layout.",
       type = "value"
      },
      acforward = {
       description = "The AC Forward key on an American layout.",
       type = "value"
      },
      achome = {
       description = "The AC Home key on an American layout.",
       type = "value"
      },
      acrefresh = {
       description = "The AC Refresh key on an American layout.",
       type = "value"
      },
      acsearch = {
       description = "The AC Search key on an American layout.",
       type = "value"
      },
      acstop = {
       description = "Th AC Stop key on an American layout.",
       type = "value"
      },
      again = {
       description = "The 'again' key on an American layout.",
       type = "value"
      },
      alterase = {
       description = "The alt-erase key on an American layout.",
       type = "value"
      },
      app1 = {
       description = "The 'app1' scancode.",
       type = "value"
      },
      app2 = {
       description = "The 'app2' scancode.",
       type = "value"
      },
      application = {
       description = "The application key on an American layout. Windows contextual menu, compose key.",
       type = "value"
      },
      audiomute = {
       description = "The audio mute key on an American layout.",
       type = "value"
      },
      audionext = {
       description = "The audio next track key on an American layout.",
       type = "value"
      },
      audioplay = {
       description = "The audio play key on an American layout.",
       type = "value"
      },
      audioprev = {
       description = "The audio previous track key on an American layout.",
       type = "value"
      },
      audiostop = {
       description = "The audio stop key on an American layout.",
       type = "value"
      },
      b = {
       description = "The 'B' key on an American layout.",
       type = "value"
      },
      backspace = {
       description = "The 'backspace' key on an American layout.",
       type = "value"
      },
      brightnessdown = {
       description = "The brightness-down scancode.",
       type = "value"
      },
      brightnessup = {
       description = "The brightness-up scancode.",
       type = "value"
      },
      c = {
       description = "The 'C' key on an American layout.",
       type = "value"
      },
      calculator = {
       description = "The calculator key on an American layout.",
       type = "value"
      },
      cancel = {
       description = "The 'cancel' key on an American layout.",
       type = "value"
      },
      capslock = {
       description = "The capslock key on an American layout.",
       type = "value"
      },
      clear = {
       description = "The 'clear' key on an American layout.",
       type = "value"
      },
      clearagain = {
       description = "The 'clearagain' key on an American layout.",
       type = "value"
      },
      computer = {
       description = "The 'computer' key on an American layout.",
       type = "value"
      },
      copy = {
       description = "The 'copy' key on an American layout.",
       type = "value"
      },
      crsel = {
       description = "The 'crsel' key on an American layout.",
       type = "value"
      },
      currencysubunit = {
       description = "The currency sub-unit key on an American layout.",
       type = "value"
      },
      currencyunit = {
       description = "The currency unit key on an American layout.",
       type = "value"
      },
      cut = {
       description = "The 'cut' key on an American layout.",
       type = "value"
      },
      d = {
       description = "The 'D' key on an American layout.",
       type = "value"
      },
      decimalseparator = {
       description = "The decimal separator key on an American layout.",
       type = "value"
      },
      delete = {
       description = "The forward-delete key on an American layout.",
       type = "value"
      },
      displayswitch = {
       description = "The display switch scancode.",
       type = "value"
      },
      down = {
       description = "The down-arrow key on an American layout.",
       type = "value"
      },
      e = {
       description = "The 'E' key on an American layout.",
       type = "value"
      },
      eject = {
       description = "The eject scancode.",
       type = "value"
      },
      ["end"] = {
       description = "The end key on an American layout.",
       type = "value"
      },
      escape = {
       description = "The 'escape' key on an American layout.",
       type = "value"
      },
      execute = {
       description = "The 'execute' key on an American layout.",
       type = "value"
      },
      exsel = {
       description = "The 'exsel' key on an American layout.",
       type = "value"
      },
      f = {
       description = "The 'F' key on an American layout.",
       type = "value"
      },
      f1 = {
       description = "The F1 key on an American layout.",
       type = "value"
      },
      f2 = {
       description = "The F2 key on an American layout.",
       type = "value"
      },
      f3 = {
       description = "The F3 key on an American layout.",
       type = "value"
      },
      f4 = {
       description = "The F4 key on an American layout.",
       type = "value"
      },
      f5 = {
       description = "The F5 key on an American layout.",
       type = "value"
      },
      f6 = {
       description = "The F6 key on an American layout.",
       type = "value"
      },
      f7 = {
       description = "The F7 key on an American layout.",
       type = "value"
      },
      f8 = {
       description = "The F8 key on an American layout.",
       type = "value"
      },
      f9 = {
       description = "The F9 key on an American layout.",
       type = "value"
      },
      f10 = {
       description = "The F10 key on an American layout.",
       type = "value"
      },
      f11 = {
       description = "The F11 key on an American layout.",
       type = "value"
      },
      f12 = {
       description = "The F12 key on an American layout.",
       type = "value"
      },
      f13 = {
       description = "The F13 key on an American layout.",
       type = "value"
      },
      f14 = {
       description = "The F14 key on an American layout.",
       type = "value"
      },
      f15 = {
       description = "The F15 key on an American layout.",
       type = "value"
      },
      f16 = {
       description = "The F16 key on an American layout.",
       type = "value"
      },
      f17 = {
       description = "The F17 key on an American layout.",
       type = "value"
      },
      f18 = {
       description = "The F18 key on an American layout.",
       type = "value"
      },
      f19 = {
       description = "The F19 key on an American layout.",
       type = "value"
      },
      f20 = {
       description = "The F20 key on an American layout.",
       type = "value"
      },
      f21 = {
       description = "The F21 key on an American layout.",
       type = "value"
      },
      f22 = {
       description = "The F22 key on an American layout.",
       type = "value"
      },
      f23 = {
       description = "The F23 key on an American layout.",
       type = "value"
      },
      f24 = {
       description = "The F24 key on an American layout.",
       type = "value"
      },
      find = {
       description = "The 'find' key on an American layout.",
       type = "value"
      },
      g = {
       description = "The 'G' key on an American layout.",
       type = "value"
      },
      h = {
       description = "The 'H' key on an American layout.",
       type = "value"
      },
      help = {
       description = "The 'help' key on an American layout.",
       type = "value"
      },
      home = {
       description = "The home key on an American layout.",
       type = "value"
      },
      i = {
       description = "The 'I' key on an American layout.",
       type = "value"
      },
      insert = {
       description = "The insert key on an American layout.",
       type = "value"
      },
      international1 = {
       description = "The 1st international key on an American layout. Used on Asian keyboards.",
       type = "value"
      },
      international2 = {
       description = "The 2nd international key on an American layout.",
       type = "value"
      },
      international3 = {
       description = "The 3rd international  key on an American layout. Yen.",
       type = "value"
      },
      international4 = {
       description = "The 4th international key on an American layout.",
       type = "value"
      },
      international5 = {
       description = "The 5th international key on an American layout.",
       type = "value"
      },
      international6 = {
       description = "The 6th international key on an American layout.",
       type = "value"
      },
      international7 = {
       description = "The 7th international key on an American layout.",
       type = "value"
      },
      international8 = {
       description = "The 8th international key on an American layout.",
       type = "value"
      },
      international9 = {
       description = "The 9th international key on an American layout.",
       type = "value"
      },
      j = {
       description = "The 'J' key on an American layout.",
       type = "value"
      },
      k = {
       description = "The 'K' key on an American layout.",
       type = "value"
      },
      kbdillumdown = {
       description = "The keyboard illumination down scancode.",
       type = "value"
      },
      kbdillumtoggle = {
       description = "The keyboard illumination toggle scancode.",
       type = "value"
      },
      kbdillumup = {
       description = "The keyboard illumination up scancode.",
       type = "value"
      },
      ["kp*"] = {
       description = "The keypad '*' key on an American layout.",
       type = "value"
      },
      ["kp+"] = {
       description = "The keypad plus key on an American layout.",
       type = "value"
      },
      ["kp-"] = {
       description = "The keypad minus key on an American layout.",
       type = "value"
      },
      ["kp."] = {
       description = "The keypad period key on an American layout.",
       type = "value"
      },
      ["kp/"] = {
       description = "The keypad forward-slash key on an American layout.",
       type = "value"
      },
      kp00 = {
       description = "The keypad 00 key on an American layout.",
       type = "value"
      },
      kp000 = {
       description = "The keypad 000 key on an American layout.",
       type = "value"
      },
      kp0 = {
       description = "The keypad '0' key on an American layout.",
       type = "value"
      },
      kp1 = {
       description = "The keypad '1' key on an American layout.",
       type = "value"
      },
      kp2 = {
       description = "The keypad '2' key on an American layout.",
       type = "value"
      },
      kp3 = {
       description = "The keypad '3' key on an American layout.",
       type = "value"
      },
      kp4 = {
       description = "The keypad '4' key on an American layout.",
       type = "value"
      },
      kp5 = {
       description = "The keypad '5' key on an American layout.",
       type = "value"
      },
      kp6 = {
       description = "The keypad '6' key on an American layout.",
       type = "value"
      },
      kp7 = {
       description = "The keypad '7' key on an American layout.",
       type = "value"
      },
      kp8 = {
       description = "The keypad '8' key on an American layout.",
       type = "value"
      },
      kp9 = {
       description = "The keypad '9' key on an American layout.",
       type = "value"
      },
      ["kp="] = {
       description = "The keypad equals key on an American layout.",
       type = "value"
      },
      kpenter = {
       description = "The keypad enter key on an American layout.",
       type = "value"
      },
      l = {
       description = "The 'L' key on an American layout.",
       type = "value"
      },
      lalt = {
       description = "The left alt / option key on an American layout.",
       type = "value"
      },
      lang1 = {
       description = "Hangul/English toggle scancode.",
       type = "value"
      },
      lang2 = {
       description = "Hanja conversion scancode.",
       type = "value"
      },
      lang3 = {
       description = "Katakana scancode.",
       type = "value"
      },
      lang4 = {
       description = "Hiragana scancode.",
       type = "value"
      },
      lang5 = {
       description = "Zenkaku/Hankaku scancode.",
       type = "value"
      },
      lctrl = {
       description = "The left control key on an American layout.",
       type = "value"
      },
      left = {
       description = "The left-arrow key on an American layout.",
       type = "value"
      },
      lgui = {
       description = "The left GUI (command / windows / super) key on an American layout.",
       type = "value"
      },
      lshift = {
       description = "The left shift key on an American layout.",
       type = "value"
      },
      m = {
       description = "The 'M' key on an American layout.",
       type = "value"
      },
      mail = {
       description = "The Mail key on an American layout.",
       type = "value"
      },
      mediaselect = {
       description = "The media select key on an American layout.",
       type = "value"
      },
      menu = {
       description = "The 'menu' key on an American layout.",
       type = "value"
      },
      mute = {
       description = "The mute key on an American layout.",
       type = "value"
      },
      n = {
       description = "The 'N' key on an American layout.",
       type = "value"
      },
      ["nonus#"] = {
       description = "The non-U.S. hash scancode.",
       type = "value"
      },
      nonusbackslash = {
       description = "The non-U.S. backslash scancode.",
       type = "value"
      },
      numlock = {
       description = "The numlock / clear key on an American layout.",
       type = "value"
      },
      o = {
       description = "The 'O' key on an American layout.",
       type = "value"
      },
      oper = {
       description = "The 'oper' key on an American layout.",
       type = "value"
      },
      out = {
       description = "The 'out' key on an American layout.",
       type = "value"
      },
      p = {
       description = "The 'P' key on an American layout.",
       type = "value"
      },
      pagedown = {
       description = "The page-down key on an American layout.",
       type = "value"
      },
      pageup = {
       description = "The page-up key on an American layout.",
       type = "value"
      },
      paste = {
       description = "The 'paste' key on an American layout.",
       type = "value"
      },
      pause = {
       description = "The pause key on an American layout.",
       type = "value"
      },
      power = {
       description = "The system power scancode.",
       type = "value"
      },
      printscreen = {
       description = "The printscreen key on an American layout.",
       type = "value"
      },
      prior = {
       description = "The 'prior' key on an American layout.",
       type = "value"
      },
      q = {
       description = "The 'Q' key on an American layout.",
       type = "value"
      },
      r = {
       description = "The 'R' key on an American layout.",
       type = "value"
      },
      ralt = {
       description = "The right alt / option key on an American layout.",
       type = "value"
      },
      rctrl = {
       description = "The right control key on an American layout.",
       type = "value"
      },
      ["return"] = {
       description = "The 'return' / 'enter' key on an American layout.",
       type = "value"
      },
      return2 = {
       description = "The 'return2' key on an American layout.",
       type = "value"
      },
      rgui = {
       description = "The right GUI (command / windows / super) key on an American layout.",
       type = "value"
      },
      right = {
       description = "The right-arrow key on an American layout.",
       type = "value"
      },
      rshift = {
       description = "The right shift key on an American layout.",
       type = "value"
      },
      s = {
       description = "The 'S' key on an American layout.",
       type = "value"
      },
      scrolllock = {
       description = "The scroll-lock key on an American layout.",
       type = "value"
      },
      select = {
       description = "The 'select' key on an American layout.",
       type = "value"
      },
      separator = {
       description = "The 'separator' key on an American layout.",
       type = "value"
      },
      sleep = {
       description = "The system sleep scancode.",
       type = "value"
      },
      space = {
       description = "The spacebar on an American layout.",
       type = "value"
      },
      stop = {
       description = "The 'stop' key on an American layout.",
       type = "value"
      },
      sysreq = {
       description = "The sysreq key on an American layout.",
       type = "value"
      },
      t = {
       description = "The 'T' key on an American layout.",
       type = "value"
      },
      tab = {
       description = "The 'tab' key on an American layout.",
       type = "value"
      },
      thsousandsseparator = {
       description = "The thousands-separator key on an American layout.",
       type = "value"
      },
      u = {
       description = "The 'U' key on an American layout.",
       type = "value"
      },
      undo = {
       description = "The 'undo' key on an American layout.",
       type = "value"
      },
      unknown = {
       description = "An unknown key.",
       type = "value"
      },
      up = {
       description = "The up-arrow key on an American layout.",
       type = "value"
      },
      v = {
       description = "The 'V' key on an American layout.",
       type = "value"
      },
      volumedown = {
       description = "The volume down key on an American layout.",
       type = "value"
      },
      volumeup = {
       description = "The volume up key on an American layout.",
       type = "value"
      },
      w = {
       description = "The 'W' key on an American layout.",
       type = "value"
      },
      www = {
       description = "The 'WWW' key on an American layout.",
       type = "value"
      },
      x = {
       description = "The 'X' key on an American layout.",
       type = "value"
      },
      y = {
       description = "The 'Y' key on an American layout.",
       type = "value"
      },
      z = {
       description = "The 'Z' key on an American layout.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    getScancodeFromKey = {
     args = "(key: KeyConstant)",
     description = "Gets the hardware scancode corresponding to the given key.\n\nUnlike key constants, Scancodes are keyboard layout-independent. For example the scancode 'w' will be generated if the key in the same place as the 'w' key on an American keyboard is pressed, no matter what the key is labelled or what the user's operating system settings are.\n\nScancodes are useful for creating default controls that have the same physical locations on on all systems.",
     returns = "(scancode: Scancode)",
     type = "function"
    },
    hasKeyRepeat = {
     args = "()",
     description = "Gets whether key repeat is enabled.",
     returns = "(enabled: boolean)",
     type = "function"
    },
    hasTextInput = {
     args = "()",
     description = "Gets whether text input events are enabled.",
     returns = "(enabled: boolean)",
     type = "function"
    },
    isDown = {
     args = "(key: KeyConstant)",
     description = "Checks whether a certain key is down. Not to be confused with love.keypressed or love.keyreleased.",
     returns = "(down: boolean)",
     type = "function"
    },
    isScancodeDown = {
     args = "(scancode: Scancode, ...: Scancode)",
     description = "Checks whether the specified Scancodes are pressed. Not to be confused with love.keypressed or love.keyreleased.\n\nUnlike regular KeyConstants, Scancodes are keyboard layout-independent. The scancode 'w' is used if the key in the same place as the 'w' key on an American keyboard is pressed, no matter what the key is labelled or what the user's operating system settings are.",
     returns = "(down: boolean)",
     type = "function"
    },
    setKeyRepeat = {
     args = "(enable: boolean)",
     description = "Enables or disables key repeat for love.keypressed. It is disabled by default.",
     returns = "()",
     type = "function"
    },
    setTextInput = {
     args = "(enable: boolean)",
     description = "Enables or disables text input events. It is enabled by default on Windows, Mac, and Linux, and disabled by default on iOS and Android.\n\nOn touch devices, this shows the system's native on-screen keyboard when it's enabled.",
     returns = "()",
     type = "function"
    }
   },
   description = "Provides an interface to the user's keyboard.",
   type = "class"
  },
  keypressed = {
   args = "(key: KeyConstant, scancode: Scancode, isrepeat: boolean)",
   description = "Callback function triggered when a key is pressed.",
   returns = "()",
   type = "function"
  },
  keyreleased = {
   args = "(key: KeyConstant, scancode: Scancode)",
   description = "Callback function triggered when a keyboard key is released.",
   returns = "()",
   type = "function"
  },
  load = {
   args = "(arg: table, unfilteredArg: table)",
   description = "This function is called exactly once at the beginning of the game.",
   returns = "()",
   type = "function"
  },
  lowmemory = {
   args = "()",
   description = "Callback function triggered when the system is running out of memory on mobile devices.\n\nMobile operating systems may forcefully kill the game if it uses too much memory, so any non-critical resource should be removed if possible (by setting all variables referencing the resources to '''nil'''), when this event is triggered. Sounds and images in particular tend to use the most memory.",
   returns = "()",
   type = "function"
  },
  math = {
   childs = {
    BezierCurve = {
     childs = {
      getControlPoint = {
       args = "(i: number)",
       description = "Get coordinates of the i-th control point. Indices start with 1.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      getControlPointCount = {
       args = "()",
       description = "Get the number of control points in the Bézier curve.",
       returns = "(count: number)",
       type = "function"
      },
      getDegree = {
       args = "()",
       description = "Get degree of the Bézier curve. The degree is equal to number-of-control-points - 1.",
       returns = "(degree: number)",
       type = "function"
      },
      getDerivative = {
       args = "()",
       description = "Get the derivative of the Bézier curve.\n\nThis function can be used to rotate sprites moving along a curve in the direction of the movement and compute the direction perpendicular to the curve at some parameter t.",
       returns = "(derivative: BezierCurve)",
       type = "function",
       valuetype = "love.math.BezierCurve"
      },
      getSegment = {
       args = "(startpoint: number, endpoint: number)",
       description = "Gets a BezierCurve that corresponds to the specified segment of this BezierCurve.",
       returns = "(curve: BezierCurve)",
       type = "function",
       valuetype = "love.math.BezierCurve"
      },
      insertControlPoint = {
       args = "(x: number, y: number, i: number)",
       description = "Insert control point as the new i-th control point. Existing control points from i onwards are pushed back by 1. Indices start with 1. Negative indices wrap around: -1 is the last control point, -2 the one before the last, etc.",
       returns = "()",
       type = "function"
      },
      removeControlPoint = {
       args = "(index: number)",
       description = "Removes the specified control point.",
       returns = "()",
       type = "function"
      },
      render = {
       args = "(depth: number)",
       description = "Get a list of coordinates to be used with love.graphics.line.\n\nThis function samples the Bézier curve using recursive subdivision. You can control the recursion depth using the depth parameter.\n\nIf you are just interested to know the position on the curve given a parameter, use BezierCurve:evaluate.",
       returns = "(coordinates: table)",
       type = "function"
      },
      renderSegment = {
       args = "(startpoint: number, endpoint: number, depth: number)",
       description = "Get a list of coordinates on a specific part of the curve, to be used with love.graphics.line.\n\nThis function samples the Bézier curve using recursive subdivision. You can control the recursion depth using the depth parameter.\n\nIf you are just need to know the position on the curve given a parameter, use BezierCurve:evaluate.",
       returns = "(coordinates: table)",
       type = "function"
      },
      rotate = {
       args = "(angle: number, ox: number, oy: number)",
       description = "Rotate the Bézier curve by an angle.",
       returns = "()",
       type = "function"
      },
      scale = {
       args = "(s: number, ox: number, oy: number)",
       description = "Scale the Bézier curve by a factor.",
       returns = "()",
       type = "function"
      },
      setControlPoint = {
       args = "(i: number, x: number, y: number)",
       description = "Set coordinates of the i-th control point. Indices start with 1.",
       returns = "()",
       type = "function"
      },
      translate = {
       args = "(dx: number, dy: number)",
       description = "Move the Bézier curve by an offset.",
       returns = "()",
       type = "function"
      }
     },
     description = "A Bézier curve object that can evaluate and render Bézier curves of arbitrary degree.\n\nFor more information on Bézier curves check this great article on Wikipedia.",
     inherits = "love.Object",
     type = "class"
    },
    CompressedData = {
     childs = {
      getFormat = {
       args = "()",
       description = "Gets the compression format of the CompressedData.",
       returns = "(format: CompressedDataFormat)",
       type = "function"
      }
     },
     description = "Represents byte data compressed using a specific algorithm.\n\nlove.data.decompress can be used to de-compress the data (or love.math.decompress in 0.10.2 or earlier).",
     inherits = "love.Data, love.Object",
     type = "class"
    },
    CompressedDataFormat = {
     childs = {
      deflate = {
       description = "Raw DEFLATE-compressed data (no header).",
       type = "value"
      },
      gzip = {
       description = "The gzip format is DEFLATE-compressed data with a slightly larger header than zlib. Since it uses DEFLATE it has the same compression characteristics as the zlib format.",
       type = "value"
      },
      lz4 = {
       description = "The LZ4 compression format. Compresses and decompresses very quickly, but the compression ratio is not the best. LZ4-HC is used when compression level 9 is specified. Some benchmarks are available here.",
       type = "value"
      },
      zlib = {
       description = "The zlib format is DEFLATE-compressed data with a small bit of header data. Compresses relatively slowly and decompresses moderately quickly, and has a decent compression ratio.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    MatrixLayout = {
     childs = {
      column = {
       description = "The matrix is column-major:",
       type = "value"
      },
      row = {
       description = "The matrix is row-major:",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    RandomGenerator = {
     childs = {
      getState = {
       args = "()",
       description = "Gets the current state of the random number generator. This returns an opaque string which is only useful for later use with RandomGenerator:setState in the same major version of LÖVE.\n\nThis is different from RandomGenerator:getSeed in that getState gets the RandomGenerator's current state, whereas getSeed gets the previously set seed number.",
       returns = "(state: string)",
       type = "function"
      },
      random = {
       args = "(max: number)",
       description = "Generates a pseudo-random number in a platform independent manner.",
       returns = "(number: number)",
       type = "function"
      },
      randomNormal = {
       args = "(stddev: number, mean: number)",
       description = "Get a normally distributed pseudo random number.",
       returns = "(number: number)",
       type = "function"
      },
      setSeed = {
       args = "(seed: number)",
       description = "Sets the seed of the random number generator using the specified integer number.",
       returns = "()",
       type = "function"
      },
      setState = {
       args = "(state: string)",
       description = "Sets the current state of the random number generator. The value used as an argument for this function is an opaque string and should only originate from a previous call to RandomGenerator:getState in the same major version of LÖVE.\n\nThis is different from RandomGenerator:setSeed in that setState directly sets the RandomGenerator's current implementation-dependent state, whereas setSeed gives it a new seed value.",
       returns = "()",
       type = "function"
      }
     },
     description = "A random number generation object which has its own random state.",
     inherits = "love.Object",
     type = "class"
    },
    Transform = {
     childs = {
      clone = {
       args = "()",
       description = "Creates a new copy of this Transform.",
       returns = "(clone: Transform)",
       type = "function",
       valuetype = "love.math.Transform"
      },
      getMatrix = {
       args = "()",
       description = "Gets the internal 4x4 transformation matrix stored by this Transform. The matrix is returned in row-major order.",
       returns = "(e1_1: number, e1_2: number, ...: number, e4_4: number)",
       type = "function"
      },
      inverse = {
       args = "()",
       description = "Creates a new Transform containing the inverse of this Transform.",
       returns = "(inverse: Transform)",
       type = "function",
       valuetype = "love.math.Transform"
      },
      inverseTransformPoint = {
       args = "(localX: number, localY: number)",
       description = "Applies the reverse of the Transform object's transformation to the given 2D position.\n\nThis effectively converts the given position from the local coordinate space of the Transform into global coordinates.\n\nOne use of this method can be to convert a screen-space mouse position into global world coordinates, if the given Transform has transformations applied that are used for a camera system in-game.",
       returns = "(globalX: number, globalY: number)",
       type = "function"
      },
      isAffine = {
       args = "()",
       description = "Checks whether the Transform is an affine transformation.",
       returns = "(affine: boolean)",
       type = "function"
      },
      reset = {
       args = "()",
       description = "Resets the Transform to an identity state. All previously applied transformations are erased.",
       returns = "(transform: Transform)",
       type = "function",
       valuetype = "love.math.Transform"
      },
      rotate = {
       args = "(angle: number)",
       description = "Applies a rotation to the Transform's coordinate system. This method does not reset any previously applied transformations.",
       returns = "(transform: Transform)",
       type = "function",
       valuetype = "love.math.Transform"
      },
      scale = {
       args = "(sx: number, sy: number)",
       description = "Scales the Transform's coordinate system. This method does not reset any previously applied transformations.",
       returns = "(transform: Transform)",
       type = "function",
       valuetype = "love.math.Transform"
      },
      setMatrix = {
       args = "(e1_1: number, e1_2: number, ...: number, e4_4: number)",
       description = "Directly sets the Transform's internal 4x4 transformation matrix.",
       returns = "(transform: Transform)",
       type = "function",
       valuetype = "love.math.Transform"
      },
      setTransformation = {
       args = "(x: number, y: number, angle: number, sx: number, sy: number, ox: number, oy: number, kx: number, ky: number)",
       description = "Resets the Transform to the specified transformation parameters.",
       returns = "(transform: Transform)",
       type = "function",
       valuetype = "love.math.Transform"
      },
      shear = {
       args = "(kx: number, ky: number)",
       description = "Applies a shear factor (skew) to the Transform's coordinate system. This method does not reset any previously applied transformations.",
       returns = "(transform: Transform)",
       type = "function",
       valuetype = "love.math.Transform"
      },
      transformPoint = {
       args = "(globalX: number, globalY: number)",
       description = "Applies the Transform object's transformation to the given 2D position.\n\nThis effectively converts the given position from global coordinates into the local coordinate space of the Transform.",
       returns = "(localX: number, localY: number)",
       type = "function"
      },
      translate = {
       args = "(dx: number, dy: number)",
       description = "Applies a translation to the Transform's coordinate system. This method does not reset any previously applied transformations.",
       returns = "(transform: Transform)",
       type = "function",
       valuetype = "love.math.Transform"
      }
     },
     description = "Object containing a coordinate system transformation.\n\nThe love.graphics module has several functions and function variants which accept Transform objects.",
     inherits = "love.Object",
     type = "class"
    },
    colorToBytes = {
     args = "(r: number, g: number, b: number, a: number)",
     description = "Converts a color from 0..1 to 0..255 range.",
     returns = "(rb: number, gb: number, bb: number, ab: number)",
     type = "function"
    },
    compress = {
     args = "(rawstring: string, format: CompressedDataFormat, level: number)",
     description = "Compresses a string or data using a specific compression algorithm.",
     returns = "(compressedData: CompressedData)",
     type = "function",
     valuetype = "love.math.CompressedData"
    },
    decompress = {
     args = "(compressedData: CompressedData)",
     description = "Decompresses a CompressedData or previously compressed string or Data object.",
     returns = "(rawstring: string)",
     type = "function"
    },
    gammaToLinear = {
     args = "(r: number, g: number, b: number)",
     description = "Converts a color from gamma-space (sRGB) to linear-space (RGB). This is useful when doing gamma-correct rendering and you need to do math in linear RGB in the few cases where LÖVE doesn't handle conversions automatically.\n\nRead more about gamma-correct rendering here, here, and here.\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.",
     returns = "(lr: number, lg: number, lb: number)",
     type = "function"
    },
    getRandomSeed = {
     args = "()",
     description = "Gets the seed of the random number generator.\n\nThe seed is split into two numbers due to Lua's use of doubles for all number values - doubles can't accurately represent integer  values above 2^53, but the seed can be an integer value up to 2^64.",
     returns = "(low: number, high: number)",
     type = "function"
    },
    getRandomState = {
     args = "()",
     description = "Gets the current state of the random number generator. This returns an opaque implementation-dependent string which is only useful for later use with love.math.setRandomState or RandomGenerator:setState.\n\nThis is different from love.math.getRandomSeed in that getRandomState gets the random number generator's current state, whereas getRandomSeed gets the previously set seed number.",
     returns = "(state: string)",
     type = "function"
    },
    isConvex = {
     args = "(vertices: table)",
     description = "Checks whether a polygon is convex.\n\nPolygonShapes in love.physics, some forms of Meshes, and polygons drawn with love.graphics.polygon must be simple convex polygons.",
     returns = "(convex: boolean)",
     type = "function"
    },
    linearToGamma = {
     args = "(lr: number, lg: number, lb: number)",
     description = "Converts a color from linear-space (RGB) to gamma-space (sRGB). This is useful when storing linear RGB color values in an image, because the linear RGB color space has less precision than sRGB for dark colors, which can result in noticeable color banding when drawing.\n\nIn general, colors chosen based on what they look like on-screen are already in gamma-space and should not be double-converted. Colors calculated using math are often in the linear RGB space.\n\nRead more about gamma-correct rendering here, here, and here.\n\nIn versions prior to 11.0, color component values were within the range of 0 to 255 instead of 0 to 1.",
     returns = "(cr: number, cg: number, cb: number)",
     type = "function"
    },
    newBezierCurve = {
     args = "(vertices: table)",
     description = "Creates a new BezierCurve object.\n\nThe number of vertices in the control polygon determines the degree of the curve, e.g. three vertices define a quadratic (degree 2) Bézier curve, four vertices define a cubic (degree 3) Bézier curve, etc.",
     returns = "(curve: BezierCurve)",
     type = "function",
     valuetype = "love.math.BezierCurve"
    },
    newRandomGenerator = {
     args = "(seed: number)",
     description = "Creates a new RandomGenerator object which is completely independent of other RandomGenerator objects and random functions.",
     returns = "(rng: RandomGenerator)",
     type = "function",
     valuetype = "love.math.RandomGenerator"
    },
    newTransform = {
     args = "(x: number, y: number, angle: number, sx: number, sy: number, ox: number, oy: number, kx: number, ky: number)",
     description = "Creates a new Transform object.",
     returns = "(transform: Transform)",
     type = "function",
     valuetype = "love.math.Transform"
    },
    noise = {
     args = "(x: number)",
     description = "Generates a Simplex or Perlin noise value in 1-4 dimensions. The return value will always be the same, given the same arguments.\n\nSimplex noise is closely related to Perlin noise. It is widely used for procedural content generation.\n\nThere are many webpages which discuss Perlin and Simplex noise in detail.",
     returns = "(value: number)",
     type = "function"
    },
    random = {
     args = "(max: number)",
     description = "Generates a pseudo-random number in a platform independent manner. The default love.run seeds this function at startup, so you generally don't need to seed it yourself.",
     returns = "(number: number)",
     type = "function"
    },
    randomNormal = {
     args = "(stddev: number, mean: number)",
     description = "Get a normally distributed pseudo random number.",
     returns = "(number: number)",
     type = "function"
    },
    setRandomSeed = {
     args = "(seed: number)",
     description = "Sets the seed of the random number generator using the specified integer number. This is called internally at startup, so you generally don't need to call it yourself.",
     returns = "()",
     type = "function"
    },
    setRandomState = {
     args = "(state: string)",
     description = "Sets the current state of the random number generator. The value used as an argument for this function is an opaque implementation-dependent string and should only originate from a previous call to love.math.getRandomState.\n\nThis is different from love.math.setRandomSeed in that setRandomState directly sets the random number generator's current implementation-dependent state, whereas setRandomSeed gives it a new seed value.",
     returns = "()",
     type = "function"
    },
    triangulate = {
     args = "(polygon: table)",
     description = "Decomposes a simple convex or concave polygon into triangles.",
     returns = "(triangles: table)",
     type = "function"
    }
   },
   description = "Provides system-independent mathematical functions.",
   type = "class"
  },
  mouse = {
   childs = {
    Cursor = {
     childs = {
      getType = {
       args = "()",
       description = "Gets the type of the Cursor.",
       returns = "(ctype: CursorType)",
       type = "function"
      }
     },
     description = "Represents a hardware cursor.",
     inherits = "love.Object",
     type = "class"
    },
    CursorType = {
     childs = {
      arrow = {
       description = "An arrow pointer.",
       type = "value"
      },
      crosshair = {
       description = "Crosshair symbol.",
       type = "value"
      },
      hand = {
       description = "Hand symbol.",
       type = "value"
      },
      ibeam = {
       description = "An I-beam, normally used when mousing over editable or selectable text.",
       type = "value"
      },
      image = {
       description = "The cursor is using a custom image.",
       type = "value"
      },
      no = {
       description = "Slashed circle or crossbones.",
       type = "value"
      },
      sizeall = {
       description = "Four-pointed arrow pointing up, down, left, and right.",
       type = "value"
      },
      sizenesw = {
       description = "Double arrow pointing to the top-right and bottom-left.",
       type = "value"
      },
      sizens = {
       description = "Double arrow pointing up and down.",
       type = "value"
      },
      sizenwse = {
       description = "Double arrow pointing to the top-left and bottom-right.",
       type = "value"
      },
      sizewe = {
       description = "Double arrow pointing left and right.",
       type = "value"
      },
      wait = {
       description = "Wait graphic.",
       type = "value"
      },
      waitarrow = {
       description = "Small wait cursor with an arrow pointer.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    getPosition = {
     args = "()",
     description = "Returns the current position of the mouse.",
     returns = "(x: number, y: number)",
     type = "function"
    },
    getRelativeMode = {
     args = "()",
     description = "Gets whether relative mode is enabled for the mouse.\n\nIf relative mode is enabled, the cursor is hidden and doesn't move when the mouse does, but relative mouse motion events are still generated via love.mousemoved. This lets the mouse move in any direction indefinitely without the cursor getting stuck at the edges of the screen.\n\nThe reported position of the mouse is not updated while relative mode is enabled, even when relative mouse motion events are generated.",
     returns = "(enabled: boolean)",
     type = "function"
    },
    getSystemCursor = {
     args = "(ctype: CursorType)",
     description = "Gets a Cursor object representing a system-native hardware cursor.\n\nHardware cursors are framerate-independent and work the same way as normal operating system cursors. Unlike drawing an image at the mouse's current coordinates, hardware cursors never have visible lag between when the mouse is moved and when the cursor position updates, even at low framerates.",
     returns = "(cursor: Cursor)",
     type = "function",
     valuetype = "love.mouse.Cursor"
    },
    getX = {
     args = "()",
     description = "Returns the current x-position of the mouse.",
     returns = "(x: number)",
     type = "function"
    },
    getY = {
     args = "()",
     description = "Returns the current y-position of the mouse.",
     returns = "(y: number)",
     type = "function"
    },
    hasCursor = {
     args = "()",
     description = "Gets whether cursor functionality is supported.\n\nIf it isn't supported, calling love.mouse.newCursor and love.mouse.getSystemCursor will cause an error. Mobile devices do not support cursors.",
     returns = "(hascursor: boolean)",
     type = "function"
    },
    isCursorSupported = {
     args = "()",
     description = "Gets whether cursor functionality is supported.\n\nIf it isn't supported, calling love.mouse.newCursor and love.mouse.getSystemCursor will cause an error. Mobile devices do not support cursors.",
     returns = "(supported: boolean)",
     type = "function"
    },
    isDown = {
     args = "(button: number, ...: number)",
     description = "Checks whether a certain mouse button is down.\n\nThis function does not detect mouse wheel scrolling; you must use the love.wheelmoved (or love.mousepressed in version 0.9.2 and older) callback for that. ",
     returns = "(down: boolean)",
     type = "function"
    },
    isGrabbed = {
     args = "()",
     description = "Checks if the mouse is grabbed.",
     returns = "(grabbed: boolean)",
     type = "function"
    },
    isVisible = {
     args = "()",
     description = "Checks if the cursor is visible.",
     returns = "(visible: boolean)",
     type = "function"
    },
    newCursor = {
     args = "(imageData: ImageData, hotx: number, hoty: number)",
     description = "Creates a new hardware Cursor object from an image file or ImageData.\n\nHardware cursors are framerate-independent and work the same way as normal operating system cursors. Unlike drawing an image at the mouse's current coordinates, hardware cursors never have visible lag between when the mouse is moved and when the cursor position updates, even at low framerates.\n\nThe hot spot is the point the operating system uses to determine what was clicked and at what position the mouse cursor is. For example, the normal arrow pointer normally has its hot spot at the top left of the image, but a crosshair cursor might have it in the middle.",
     returns = "(cursor: Cursor)",
     type = "function",
     valuetype = "love.mouse.Cursor"
    },
    setCursor = {
     args = "(cursor: Cursor)",
     description = "Sets the current mouse cursor.",
     returns = "()",
     type = "function"
    },
    setGrabbed = {
     args = "(grab: boolean)",
     description = "Grabs the mouse and confines it to the window.",
     returns = "()",
     type = "function"
    },
    setPosition = {
     args = "(x: number, y: number)",
     description = "Sets the current position of the mouse. Non-integer values are floored.",
     returns = "()",
     type = "function"
    },
    setRelativeMode = {
     args = "(enable: boolean)",
     description = "Sets whether relative mode is enabled for the mouse.\n\nWhen relative mode is enabled, the cursor is hidden and doesn't move when the mouse does, but relative mouse motion events are still generated via love.mousemoved. This lets the mouse move in any direction indefinitely without the cursor getting stuck at the edges of the screen.\n\nThe reported position of the mouse may not be updated while relative mode is enabled, even when relative mouse motion events are generated.",
     returns = "()",
     type = "function"
    },
    setVisible = {
     args = "(visible: boolean)",
     description = "Sets the current visibility of the cursor.",
     returns = "()",
     type = "function"
    },
    setX = {
     args = "(x: number)",
     description = "Sets the current X position of the mouse.\n\nNon-integer values are floored.",
     returns = "()",
     type = "function"
    },
    setY = {
     args = "(y: number)",
     description = "Sets the current Y position of the mouse.\n\nNon-integer values are floored.",
     returns = "()",
     type = "function"
    }
   },
   description = "Provides an interface to the user's mouse.",
   type = "class"
  },
  mousefocus = {
   args = "(focus: boolean)",
   description = "Callback function triggered when window receives or loses mouse focus.",
   returns = "()",
   type = "function"
  },
  mousemoved = {
   args = "(x: number, y: number, dx: number, dy: number, istouch: boolean)",
   description = "Callback function triggered when the mouse is moved.",
   returns = "()",
   type = "function"
  },
  mousepressed = {
   args = "(x: number, y: number, button: number, istouch: boolean, presses: number)",
   description = "Callback function triggered when a mouse button is pressed.",
   returns = "()",
   type = "function"
  },
  mousereleased = {
   args = "(x: number, y: number, button: number, istouch: boolean, presses: number)",
   description = "Callback function triggered when a mouse button is released.",
   returns = "()",
   type = "function"
  },
  physics = {
   childs = {
    Body = {
     childs = {
      applyForce = {
       args = "(fx: number, fy: number)",
       description = "Apply force to a Body.\n\nA force pushes a body in a direction. A body with with a larger mass will react less. The reaction also depends on how long a force is applied: since the force acts continuously over the entire timestep, a short timestep will only push the body for a short time. Thus forces are best used for many timesteps to give a continuous push to a body (like gravity). For a single push that is independent of timestep, it is better to use Body:applyLinearImpulse.\n\nIf the position to apply the force is not given, it will act on the center of mass of the body. The part of the force not directed towards the center of mass will cause the body to spin (and depends on the rotational inertia).\n\nNote that the force components and position must be given in world coordinates.",
       returns = "()",
       type = "function"
      },
      applyLinearImpulse = {
       args = "(ix: number, iy: number)",
       description = "Applies an impulse to a body.\n\nThis makes a single, instantaneous addition to the body momentum.\n\nAn impulse pushes a body in a direction. A body with with a larger mass will react less. The reaction does '''not''' depend on the timestep, and is equivalent to applying a force continuously for 1 second. Impulses are best used to give a single push to a body. For a continuous push to a body it is better to use Body:applyForce.\n\nIf the position to apply the impulse is not given, it will act on the center of mass of the body. The part of the impulse not directed towards the center of mass will cause the body to spin (and depends on the rotational inertia). \n\nNote that the impulse components and position must be given in world coordinates.",
       returns = "()",
       type = "function"
      },
      applyTorque = {
       args = "(torque: number)",
       description = "Apply torque to a body.\n\nTorque is like a force that will change the angular velocity (spin) of a body. The effect will depend on the rotational inertia a body has.",
       returns = "()",
       type = "function"
      },
      destroy = {
       args = "()",
       description = "Explicitly destroys the Body and all fixtures and joints attached to it.\n\nAn error will occur if you attempt to use the object after calling this function. In 0.7.2, when you don't have time to wait for garbage collection, this function may be used to free the object immediately.",
       returns = "()",
       type = "function"
      },
      getAngle = {
       args = "()",
       description = "Get the angle of the body.\n\nThe angle is measured in radians. If you need to transform it to degrees, use math.deg.\n\nA value of 0 radians will mean 'looking to the right'. Although radians increase counter-clockwise, the y axis points down so it becomes ''clockwise'' from our point of view.",
       returns = "(angle: number)",
       type = "function"
      },
      getAngularDamping = {
       args = "()",
       description = "Gets the Angular damping of the Body\n\nThe angular damping is the ''rate of decrease of the angular velocity over time'': A spinning body with no damping and no external forces will continue spinning indefinitely. A spinning body with damping will gradually stop spinning.\n\nDamping is not the same as friction - they can be modelled together. However, only damping is provided by Box2D (and LOVE).\n\nDamping parameters should be between 0 and infinity, with 0 meaning no damping, and infinity meaning full damping. Normally you will use a damping value between 0 and 0.1.",
       returns = "(damping: number)",
       type = "function"
      },
      getAngularVelocity = {
       args = "()",
       description = "Get the angular velocity of the Body.\n\nThe angular velocity is the ''rate of change of angle over time''.\n\nIt is changed in World:update by applying torques, off centre forces/impulses, and angular damping. It can be set directly with Body:setAngularVelocity.\n\nIf you need the ''rate of change of position over time'', use Body:getLinearVelocity.",
       returns = "(w: number)",
       type = "function"
      },
      getContacts = {
       args = "()",
       description = "Gets a list of all Contacts attached to the Body.",
       returns = "(contacts: table)",
       type = "function"
      },
      getFixtures = {
       args = "()",
       description = "Returns a table with all fixtures.",
       returns = "(fixtures: table)",
       type = "function"
      },
      getGravityScale = {
       args = "()",
       description = "Returns the gravity scale factor.",
       returns = "(scale: number)",
       type = "function"
      },
      getInertia = {
       args = "()",
       description = "Gets the rotational inertia of the body.\n\nThe rotational inertia is how hard is it to make the body spin.",
       returns = "(inertia: number)",
       type = "function"
      },
      getJoints = {
       args = "()",
       description = "Returns a table containing the Joints attached to this Body.",
       returns = "(joints: table)",
       type = "function"
      },
      getLinearDamping = {
       args = "()",
       description = "Gets the linear damping of the Body.\n\nThe linear damping is the ''rate of decrease of the linear velocity over time''. A moving body with no damping and no external forces will continue moving indefinitely, as is the case in space. A moving body with damping will gradually stop moving.\n\nDamping is not the same as friction - they can be modelled together.",
       returns = "(damping: number)",
       type = "function"
      },
      getLinearVelocity = {
       args = "()",
       description = "Gets the linear velocity of the Body from its center of mass.\n\nThe linear velocity is the ''rate of change of position over time''.\n\nIf you need the ''rate of change of angle over time'', use Body:getAngularVelocity.\n\nIf you need to get the linear velocity of a point different from the center of mass:\n\n*  Body:getLinearVelocityFromLocalPoint allows you to specify the point in local coordinates.\n\n*  Body:getLinearVelocityFromWorldPoint allows you to specify the point in world coordinates.\n\nSee page 136 of 'Essential Mathematics for Games and Interactive Applications' for definitions of local and world coordinates.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      getLinearVelocityFromLocalPoint = {
       args = "(x: number, y: number)",
       description = "Get the linear velocity of a point on the body.\n\nThe linear velocity for a point on the body is the velocity of the body center of mass plus the velocity at that point from the body spinning.\n\nThe point on the body must given in local coordinates. Use Body:getLinearVelocityFromWorldPoint to specify this with world coordinates.",
       returns = "(vx: number, vy: number)",
       type = "function"
      },
      getLinearVelocityFromWorldPoint = {
       args = "(x: number, y: number)",
       description = "Get the linear velocity of a point on the body.\n\nThe linear velocity for a point on the body is the velocity of the body center of mass plus the velocity at that point from the body spinning.\n\nThe point on the body must given in world coordinates. Use Body:getLinearVelocityFromLocalPoint to specify this with local coordinates.",
       returns = "(vx: number, vy: number)",
       type = "function"
      },
      getLocalCenter = {
       args = "()",
       description = "Get the center of mass position in local coordinates.\n\nUse Body:getWorldCenter to get the center of mass in world coordinates.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      getLocalPoint = {
       args = "(worldX: number, worldY: number)",
       description = "Transform a point from world coordinates to local coordinates.",
       returns = "(localX: number, localY: number)",
       type = "function"
      },
      getLocalVector = {
       args = "(worldX: number, worldY: number)",
       description = "Transform a vector from world coordinates to local coordinates.",
       returns = "(localX: number, localY: number)",
       type = "function"
      },
      getMass = {
       args = "()",
       description = "Get the mass of the body.\n\nStatic bodies always have a mass of 0.",
       returns = "(mass: number)",
       type = "function"
      },
      getMassData = {
       args = "()",
       description = "Returns the mass, its center, and the rotational inertia.",
       returns = "(x: number, y: number, mass: number, inertia: number)",
       type = "function"
      },
      getPosition = {
       args = "()",
       description = "Get the position of the body.\n\nNote that this may not be the center of mass of the body.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      getTransform = {
       args = "()",
       description = "Get the position and angle of the body.\n\nNote that the position may not be the center of mass of the body. An angle of 0 radians will mean 'looking to the right'. Although radians increase counter-clockwise, the y axis points down so it becomes clockwise from our point of view.",
       returns = "(x: number, y: number, angle: number)",
       type = "function"
      },
      getType = {
       args = "()",
       description = "Returns the type of the body.",
       returns = "(type: BodyType)",
       type = "function"
      },
      getUserData = {
       args = "()",
       description = "Returns the Lua value associated with this Body.",
       returns = "(value: any)",
       type = "function"
      },
      getWorld = {
       args = "()",
       description = "Gets the World the body lives in.",
       returns = "(world: World)",
       type = "function",
       valuetype = "love.physics.World"
      },
      getWorldCenter = {
       args = "()",
       description = "Get the center of mass position in world coordinates.\n\nUse Body:getLocalCenter to get the center of mass in local coordinates.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      getWorldPoint = {
       args = "(localX: number, localY: number)",
       description = "Transform a point from local coordinates to world coordinates.",
       returns = "(worldX: number, worldY: number)",
       type = "function"
      },
      getWorldPoints = {
       args = "(x1: number, y1: number, x2: number, y2: number)",
       description = "Transforms multiple points from local coordinates to world coordinates.",
       returns = "(x1: number, y1: number, x2: number, y2: number)",
       type = "function"
      },
      getWorldVector = {
       args = "(localX: number, localY: number)",
       description = "Transform a vector from local coordinates to world coordinates.",
       returns = "(worldX: number, worldY: number)",
       type = "function"
      },
      getX = {
       args = "()",
       description = "Get the x position of the body in world coordinates.",
       returns = "(x: number)",
       type = "function"
      },
      getY = {
       args = "()",
       description = "Get the y position of the body in world coordinates.",
       returns = "(y: number)",
       type = "function"
      },
      isActive = {
       args = "()",
       description = "Returns whether the body is actively used in the simulation.",
       returns = "(status: boolean)",
       type = "function"
      },
      isAwake = {
       args = "()",
       description = "Returns the sleep status of the body.",
       returns = "(status: boolean)",
       type = "function"
      },
      isBullet = {
       args = "()",
       description = "Get the bullet status of a body.\n\nThere are two methods to check for body collisions:\n\n*  at their location when the world is updated (default)\n\n*  using continuous collision detection (CCD)\n\nThe default method is efficient, but a body moving very quickly may sometimes jump over another body without producing a collision. A body that is set as a bullet will use CCD. This is less efficient, but is guaranteed not to jump when moving quickly.\n\nNote that static bodies (with zero mass) always use CCD, so your walls will not let a fast moving body pass through even if it is not a bullet.",
       returns = "(status: boolean)",
       type = "function"
      },
      isDestroyed = {
       args = "()",
       description = "Gets whether the Body is destroyed. Destroyed bodies cannot be used.",
       returns = "(destroyed: boolean)",
       type = "function"
      },
      isFixedRotation = {
       args = "()",
       description = "Returns whether the body rotation is locked.",
       returns = "(fixed: boolean)",
       type = "function"
      },
      isSleepingAllowed = {
       args = "()",
       description = "Returns the sleeping behaviour of the body.",
       returns = "(allowed: boolean)",
       type = "function"
      },
      isTouching = {
       args = "(otherbody: Body)",
       description = "Gets whether the Body is touching the given other Body.",
       returns = "(touching: boolean)",
       type = "function"
      },
      resetMassData = {
       args = "()",
       description = "Resets the mass of the body by recalculating it from the mass properties of the fixtures.",
       returns = "()",
       type = "function"
      },
      setActive = {
       args = "(active: boolean)",
       description = "Sets whether the body is active in the world.\n\nAn inactive body does not take part in the simulation. It will not move or cause any collisions.",
       returns = "()",
       type = "function"
      },
      setAngle = {
       args = "(angle: number)",
       description = "Set the angle of the body.\n\nThe angle is measured in radians. If you need to transform it from degrees, use math.rad.\n\nA value of 0 radians will mean 'looking to the right'. Although radians increase counter-clockwise, the y axis points down so it becomes ''clockwise'' from our point of view.\n\nIt is possible to cause a collision with another body by changing its angle. ",
       returns = "()",
       type = "function"
      },
      setAngularDamping = {
       args = "(damping: number)",
       description = "Sets the angular damping of a Body\n\nSee Body:getAngularDamping for a definition of angular damping.\n\nAngular damping can take any value from 0 to infinity. It is recommended to stay between 0 and 0.1, though. Other values will look unrealistic.",
       returns = "()",
       type = "function"
      },
      setAngularVelocity = {
       args = "(w: number)",
       description = "Sets the angular velocity of a Body.\n\nThe angular velocity is the ''rate of change of angle over time''.\n\nThis function will not accumulate anything; any impulses previously applied since the last call to World:update will be lost. ",
       returns = "()",
       type = "function"
      },
      setAwake = {
       args = "(awake: boolean)",
       description = "Wakes the body up or puts it to sleep.",
       returns = "()",
       type = "function"
      },
      setBullet = {
       args = "(status: boolean)",
       description = "Set the bullet status of a body.\n\nThere are two methods to check for body collisions:\n\n*  at their location when the world is updated (default)\n\n*  using continuous collision detection (CCD)\n\nThe default method is efficient, but a body moving very quickly may sometimes jump over another body without producing a collision. A body that is set as a bullet will use CCD. This is less efficient, but is guaranteed not to jump when moving quickly.\n\nNote that static bodies (with zero mass) always use CCD, so your walls will not let a fast moving body pass through even if it is not a bullet.",
       returns = "()",
       type = "function"
      },
      setFixedRotation = {
       args = "(isFixed: boolean)",
       description = "Set whether a body has fixed rotation.\n\nBodies with fixed rotation don't vary the speed at which they rotate. Calling this function causes the mass to be reset. ",
       returns = "()",
       type = "function"
      },
      setGravityScale = {
       args = "(scale: number)",
       description = "Sets a new gravity scale factor for the body.",
       returns = "()",
       type = "function"
      },
      setInertia = {
       args = "(inertia: number)",
       description = "Set the inertia of a body.",
       returns = "()",
       type = "function"
      },
      setLinearDamping = {
       args = "(ld: number)",
       description = "Sets the linear damping of a Body\n\nSee Body:getLinearDamping for a definition of linear damping.\n\nLinear damping can take any value from 0 to infinity. It is recommended to stay between 0 and 0.1, though. Other values will make the objects look 'floaty'(if gravity is enabled).",
       returns = "()",
       type = "function"
      },
      setLinearVelocity = {
       args = "(x: number, y: number)",
       description = "Sets a new linear velocity for the Body.\n\nThis function will not accumulate anything; any impulses previously applied since the last call to World:update will be lost.",
       returns = "()",
       type = "function"
      },
      setMass = {
       args = "(mass: number)",
       description = "Sets a new body mass.",
       returns = "()",
       type = "function"
      },
      setMassData = {
       args = "(x: number, y: number, mass: number, inertia: number)",
       description = "Overrides the calculated mass data.",
       returns = "()",
       type = "function"
      },
      setPosition = {
       args = "(x: number, y: number)",
       description = "Set the position of the body.\n\nNote that this may not be the center of mass of the body.\n\nThis function cannot wake up the body.",
       returns = "()",
       type = "function"
      },
      setSleepingAllowed = {
       args = "(allowed: boolean)",
       description = "Sets the sleeping behaviour of the body. Should sleeping be allowed, a body at rest will automatically sleep. A sleeping body is not simulated unless it collided with an awake body. Be wary that one can end up with a situation like a floating sleeping body if the floor was removed.",
       returns = "()",
       type = "function"
      },
      setTransform = {
       args = "(x: number, y: number, angle: number)",
       description = "Set the position and angle of the body.\n\nNote that the position may not be the center of mass of the body. An angle of 0 radians will mean 'looking to the right'. Although radians increase counter-clockwise, the y axis points down so it becomes clockwise from our point of view.\n\nThis function cannot wake up the body.",
       returns = "()",
       type = "function"
      },
      setType = {
       args = "(type: BodyType)",
       description = "Sets a new body type.",
       returns = "()",
       type = "function"
      },
      setUserData = {
       args = "(value: any)",
       description = "Associates a Lua value with the Body.\n\nTo delete the reference, explicitly pass nil.",
       returns = "()",
       type = "function"
      },
      setX = {
       args = "(x: number)",
       description = "Set the x position of the body.\n\nThis function cannot wake up the body. ",
       returns = "()",
       type = "function"
      },
      setY = {
       args = "(y: number)",
       description = "Set the y position of the body.\n\nThis function cannot wake up the body. ",
       returns = "()",
       type = "function"
      }
     },
     description = "Bodies are objects with velocity and position.",
     inherits = "love.Object",
     type = "class"
    },
    BodyType = {
     childs = {
      dynamic = {
       description = "Dynamic bodies collide with all bodies.",
       type = "value"
      },
      kinematic = {
       description = "Kinematic bodies only collide with dynamic bodies.",
       type = "value"
      },
      static = {
       description = "Static bodies do not move.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    ChainShape = {
     childs = {
      getNextVertex = {
       args = "()",
       description = "Gets the vertex that establishes a connection to the next shape.\n\nSetting next and previous ChainShape vertices can help prevent unwanted collisions when a flat shape slides along the edge and moves over to the new shape.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      getPoint = {
       args = "(index: number)",
       description = "Returns a point of the shape.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      getPoints = {
       args = "()",
       description = "Returns all points of the shape.",
       returns = "(x1: number, y1: number, x2: number, y2: number)",
       type = "function"
      },
      getPreviousVertex = {
       args = "()",
       description = "Gets the vertex that establishes a connection to the previous shape.\n\nSetting next and previous ChainShape vertices can help prevent unwanted collisions when a flat shape slides along the edge and moves over to the new shape.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      getVertexCount = {
       args = "()",
       description = "Returns the number of vertices the shape has.",
       returns = "(count: number)",
       type = "function"
      },
      setNextVertex = {
       args = "(x: number, y: number)",
       description = "Sets a vertex that establishes a connection to the next shape.\n\nThis can help prevent unwanted collisions when a flat shape slides along the edge and moves over to the new shape.",
       returns = "()",
       type = "function"
      },
      setPreviousVertex = {
       args = "(x: number, y: number)",
       description = "Sets a vertex that establishes a connection to the previous shape.\n\nThis can help prevent unwanted collisions when a flat shape slides along the edge and moves over to the new shape.",
       returns = "()",
       type = "function"
      }
     },
     description = "A ChainShape consists of multiple line segments. It can be used to create the boundaries of your terrain. The shape does not have volume and can only collide with PolygonShape and CircleShape.\n\nUnlike the PolygonShape, the ChainShape does not have a vertices limit or has to form a convex shape, but self intersections are not supported.",
     inherits = "love.physics.Shape, love.Object",
     type = "class"
    },
    CircleShape = {
     childs = {
      getRadius = {
       args = "()",
       description = "Gets the radius of the circle shape.",
       returns = "(radius: number)",
       type = "function"
      },
      setPoint = {
       args = "(x: number, y: number)",
       description = "Sets the location of the center of the circle shape.",
       returns = "()",
       type = "function"
      },
      setRadius = {
       args = "(radius: number)",
       description = "Sets the radius of the circle.",
       returns = "()",
       type = "function"
      }
     },
     description = "Circle extends Shape and adds a radius and a local position.",
     inherits = "love.physics.Shape, love.Object",
     type = "class"
    },
    Contact = {
     childs = {
      getFriction = {
       args = "()",
       description = "Get the friction between two shapes that are in contact.",
       returns = "(friction: number)",
       type = "function"
      },
      getNormal = {
       args = "()",
       description = "Get the normal vector between two shapes that are in contact.\n\nThis function returns the coordinates of a unit vector that points from the first shape to the second.",
       returns = "(nx: number, ny: number)",
       type = "function"
      },
      getPositions = {
       args = "()",
       description = "Returns the contact points of the two colliding fixtures. There can be one or two points.",
       returns = "(x1: number, y1: number, x2: number, y2: number)",
       type = "function"
      },
      getRestitution = {
       args = "()",
       description = "Get the restitution between two shapes that are in contact.",
       returns = "(restitution: number)",
       type = "function"
      },
      isEnabled = {
       args = "()",
       description = "Returns whether the contact is enabled. The collision will be ignored if a contact gets disabled in the preSolve callback.",
       returns = "(enabled: boolean)",
       type = "function"
      },
      isTouching = {
       args = "()",
       description = "Returns whether the two colliding fixtures are touching each other.",
       returns = "(touching: boolean)",
       type = "function"
      },
      resetFriction = {
       args = "()",
       description = "Resets the contact friction to the mixture value of both fixtures.",
       returns = "()",
       type = "function"
      },
      resetRestitution = {
       args = "()",
       description = "Resets the contact restitution to the mixture value of both fixtures.",
       returns = "()",
       type = "function"
      },
      setEnabled = {
       args = "(enabled: boolean)",
       description = "Enables or disables the contact.",
       returns = "()",
       type = "function"
      },
      setFriction = {
       args = "(friction: number)",
       description = "Sets the contact friction.",
       returns = "()",
       type = "function"
      },
      setRestitution = {
       args = "(restitution: number)",
       description = "Sets the contact restitution.",
       returns = "()",
       type = "function"
      }
     },
     description = "Contacts are objects created to manage collisions in worlds.",
     inherits = "love.Object",
     type = "class"
    },
    DistanceJoint = {
     childs = {
      getFrequency = {
       args = "()",
       description = "Gets the response speed.",
       returns = "(Hz: number)",
       type = "function"
      },
      getLength = {
       args = "()",
       description = "Gets the equilibrium distance between the two Bodies.",
       returns = "(l: number)",
       type = "function"
      },
      setDampingRatio = {
       args = "(ratio: number)",
       description = "Sets the damping ratio.",
       returns = "()",
       type = "function"
      },
      setFrequency = {
       args = "(Hz: number)",
       description = "Sets the response speed.",
       returns = "()",
       type = "function"
      },
      setLength = {
       args = "(l: number)",
       description = "Sets the equilibrium distance between the two Bodies.",
       returns = "()",
       type = "function"
      }
     },
     description = "Keeps two bodies at the same distance.",
     inherits = "love.physics.Joint, love.Object",
     type = "class"
    },
    EdgeShape = {
     childs = {
      getPoints = {
       args = "()",
       description = "Returns the local coordinates of the edge points.",
       returns = "(x1: number, y1: number, x2: number, y2: number)",
       type = "function"
      },
      getPreviousVertex = {
       args = "()",
       description = "Gets the vertex that establishes a connection to the previous shape.\n\nSetting next and previous EdgeShape vertices can help prevent unwanted collisions when a flat shape slides along the edge and moves over to the new shape.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      setNextVertex = {
       args = "(x: number, y: number)",
       description = "Sets a vertex that establishes a connection to the next shape.\n\nThis can help prevent unwanted collisions when a flat shape slides along the edge and moves over to the new shape.",
       returns = "()",
       type = "function"
      },
      setPreviousVertex = {
       args = "(x: number, y: number)",
       description = "Sets a vertex that establishes a connection to the previous shape.\n\nThis can help prevent unwanted collisions when a flat shape slides along the edge and moves over to the new shape.",
       returns = "()",
       type = "function"
      }
     },
     description = "A EdgeShape is a line segment. They can be used to create the boundaries of your terrain. The shape does not have volume and can only collide with PolygonShape and CircleShape.",
     inherits = "love.physics.Shape, love.Object",
     type = "class"
    },
    Fixture = {
     childs = {
      getBody = {
       args = "()",
       description = "Returns the body to which the fixture is attached.",
       returns = "(body: Body)",
       type = "function",
       valuetype = "love.physics.Body"
      },
      getBoundingBox = {
       args = "(index: number)",
       description = "Returns the points of the fixture bounding box. In case the fixture has multiple children a 1-based index can be specified. For example, a fixture will have multiple children with a chain shape.",
       returns = "(topLeftX: number, topLeftY: number, bottomRightX: number, bottomRightY: number)",
       type = "function"
      },
      getCategory = {
       args = "()",
       description = "Returns the categories the fixture belongs to.",
       returns = "(category1: number, category2: number)",
       type = "function"
      },
      getDensity = {
       args = "()",
       description = "Returns the density of the fixture.",
       returns = "(density: number)",
       type = "function"
      },
      getFilterData = {
       args = "()",
       description = "Returns the filter data of the fixture.\n\nCategories and masks are encoded as the bits of a 16-bit integer.",
       returns = "(categories: number, mask: number, group: number)",
       type = "function"
      },
      getFriction = {
       args = "()",
       description = "Returns the friction of the fixture.",
       returns = "(friction: number)",
       type = "function"
      },
      getGroupIndex = {
       args = "()",
       description = "Returns the group the fixture belongs to. Fixtures with the same group will always collide if the group is positive or never collide if it's negative. The group zero means no group.\n\nThe groups range from -32768 to 32767.",
       returns = "(group: number)",
       type = "function"
      },
      getMask = {
       args = "()",
       description = "Returns which categories this fixture should '''NOT''' collide with.",
       returns = "(mask1: number, mask2: number)",
       type = "function"
      },
      getMassData = {
       args = "()",
       description = "Returns the mass, its center and the rotational inertia.",
       returns = "(x: number, y: number, mass: number, inertia: number)",
       type = "function"
      },
      getRestitution = {
       args = "()",
       description = "Returns the restitution of the fixture.",
       returns = "(restitution: number)",
       type = "function"
      },
      getShape = {
       args = "()",
       description = "Returns the shape of the fixture. This shape is a reference to the actual data used in the simulation. It's possible to change its values between timesteps.",
       returns = "(shape: Shape)",
       type = "function",
       valuetype = "love.physics.Shape"
      },
      getUserData = {
       args = "()",
       description = "Returns the Lua value associated with this fixture.",
       returns = "(value: any)",
       type = "function"
      },
      isDestroyed = {
       args = "()",
       description = "Gets whether the Fixture is destroyed. Destroyed fixtures cannot be used.",
       returns = "(destroyed: boolean)",
       type = "function"
      },
      isSensor = {
       args = "()",
       description = "Returns whether the fixture is a sensor.",
       returns = "(sensor: boolean)",
       type = "function"
      },
      rayCast = {
       args = "(x1: number, y1: number, x2: number, y2: number, maxFraction: number, childIndex: number)",
       description = "Casts a ray against the shape of the fixture and returns the surface normal vector and the line position where the ray hit. If the ray missed the shape, nil will be returned.\n\nThe ray starts on the first point of the input line and goes towards the second point of the line. The fifth argument is the maximum distance the ray is going to travel as a scale factor of the input line length.\n\nThe childIndex parameter is used to specify which child of a parent shape, such as a ChainShape, will be ray casted. For ChainShapes, the index of 1 is the first edge on the chain. Ray casting a parent shape will only test the child specified so if you want to test every shape of the parent, you must loop through all of its children.\n\nThe world position of the impact can be calculated by multiplying the line vector with the third return value and adding it to the line starting point.\n\nhitx, hity = x1 + (x2 - x1) * fraction, y1 + (y2 - y1) * fraction",
       returns = "(xn: number, yn: number, fraction: number)",
       type = "function"
      },
      setCategory = {
       args = "(category1: number, category2: number)",
       description = "Sets the categories the fixture belongs to. There can be up to 16 categories represented as a number from 1 to 16.\n\nAll fixture's default category is 1.",
       returns = "()",
       type = "function"
      },
      setDensity = {
       args = "(density: number)",
       description = "Sets the density of the fixture. Call Body:resetMassData if this needs to take effect immediately.",
       returns = "()",
       type = "function"
      },
      setFilterData = {
       args = "(categories: number, mask: number, group: number)",
       description = "Sets the filter data of the fixture.\n\nGroups, categories, and mask can be used to define the collision behaviour of the fixture.\n\nIf two fixtures are in the same group they either always collide if the group is positive, or never collide if it's negative. If the group is zero or they do not match, then the contact filter checks if the fixtures select a category of the other fixture with their masks. The fixtures do not collide if that's not the case. If they do have each other's categories selected, the return value of the custom contact filter will be used. They always collide if none was set.\n\nThere can be up to 16 categories. Categories and masks are encoded as the bits of a 16-bit integer.\n\nWhen created, prior to calling this function, all fixtures have category set to 1, mask set to 65535 (all categories) and group set to 0.\n\nThis function allows setting all filter data for a fixture at once. To set only the categories, the mask or the group, you can use Fixture:setCategory, Fixture:setMask or Fixture:setGroupIndex respectively.",
       returns = "()",
       type = "function"
      },
      setFriction = {
       args = "(friction: number)",
       description = "Sets the friction of the fixture.\n\nFriction determines how shapes react when they 'slide' along other shapes. Low friction indicates a slippery surface, like ice, while high friction indicates a rough surface, like concrete. Range: 0.0 - 1.0.",
       returns = "()",
       type = "function"
      },
      setGroupIndex = {
       args = "(group: number)",
       description = "Sets the group the fixture belongs to. Fixtures with the same group will always collide if the group is positive or never collide if it's negative. The group zero means no group.\n\nThe groups range from -32768 to 32767.",
       returns = "()",
       type = "function"
      },
      setMask = {
       args = "(mask1: number, mask2: number)",
       description = "Sets the category mask of the fixture. There can be up to 16 categories represented as a number from 1 to 16.\n\nThis fixture will '''NOT''' collide with the fixtures that are in the selected categories if the other fixture also has a category of this fixture selected.",
       returns = "()",
       type = "function"
      },
      setRestitution = {
       args = "(restitution: number)",
       description = "Sets the restitution of the fixture.",
       returns = "()",
       type = "function"
      },
      setSensor = {
       args = "(sensor: boolean)",
       description = "Sets whether the fixture should act as a sensor.\n\nSensors do not cause collision responses, but the begin-contact and end-contact World callbacks will still be called for this fixture.",
       returns = "()",
       type = "function"
      },
      setUserData = {
       args = "(value: any)",
       description = "Associates a Lua value with the fixture.\n\nTo delete the reference, explicitly pass nil.",
       returns = "()",
       type = "function"
      },
      testPoint = {
       args = "(x: number, y: number)",
       description = "Checks if a point is inside the shape of the fixture.",
       returns = "(isInside: boolean)",
       type = "function"
      }
     },
     description = "Fixtures attach shapes to bodies.",
     inherits = "love.Object",
     type = "class"
    },
    FrictionJoint = {
     childs = {
      getMaxTorque = {
       args = "()",
       description = "Gets the maximum friction torque in Newton-meters.",
       returns = "(torque: number)",
       type = "function"
      },
      setMaxForce = {
       args = "(maxForce: number)",
       description = "Sets the maximum friction force in Newtons.",
       returns = "()",
       type = "function"
      },
      setMaxTorque = {
       args = "(torque: number)",
       description = "Sets the maximum friction torque in Newton-meters.",
       returns = "()",
       type = "function"
      }
     },
     description = "A FrictionJoint applies friction to a body.",
     inherits = "love.physics.Joint, love.Object",
     type = "class"
    },
    GearJoint = {
     childs = {
      getRatio = {
       args = "()",
       description = "Get the ratio of a gear joint.",
       returns = "(ratio: number)",
       type = "function"
      },
      setRatio = {
       args = "(ratio: number)",
       description = "Set the ratio of a gear joint.",
       returns = "()",
       type = "function"
      }
     },
     description = "Keeps bodies together in such a way that they act like gears.",
     inherits = "love.physics.Joint, love.Object",
     type = "class"
    },
    Joint = {
     childs = {
      getAnchors = {
       args = "()",
       description = "Get the anchor points of the joint.",
       returns = "(x1: number, y1: number, x2: number, y2: number)",
       type = "function"
      },
      getBodies = {
       args = "()",
       description = "Gets the bodies that the Joint is attached to.",
       returns = "(bodyA: Body, bodyB: Body)",
       type = "function",
       valuetype = "love.physics.Body"
      },
      getCollideConnected = {
       args = "()",
       description = "Gets whether the connected Bodies collide.",
       returns = "(c: boolean)",
       type = "function"
      },
      getReactionForce = {
       args = "(x: number)",
       description = "Returns the reaction force in newtons on the second body",
       returns = "(x: number, y: number)",
       type = "function"
      },
      getReactionTorque = {
       args = "(invdt: number)",
       description = "Returns the reaction torque on the second body.",
       returns = "(torque: number)",
       type = "function"
      },
      getType = {
       args = "()",
       description = "Gets a string representing the type.",
       returns = "(type: JointType)",
       type = "function"
      },
      getUserData = {
       args = "()",
       description = "Returns the Lua value associated with this Joint.",
       returns = "(value: any)",
       type = "function"
      },
      isDestroyed = {
       args = "()",
       description = "Gets whether the Joint is destroyed. Destroyed joints cannot be used.",
       returns = "(destroyed: boolean)",
       type = "function"
      },
      setUserData = {
       args = "(value: any)",
       description = "Associates a Lua value with the Joint.\n\nTo delete the reference, explicitly pass nil.",
       returns = "()",
       type = "function"
      }
     },
     description = "Attach multiple bodies together to interact in unique ways.",
     inherits = "love.Object",
     type = "class"
    },
    JointType = {
     childs = {
      distance = {
       description = "A DistanceJoint.",
       type = "value"
      },
      friction = {
       description = "A FrictionJoint.",
       type = "value"
      },
      gear = {
       description = "A GearJoint.",
       type = "value"
      },
      mouse = {
       description = "A MouseJoint.",
       type = "value"
      },
      prismatic = {
       description = "A PrismaticJoint.",
       type = "value"
      },
      pulley = {
       description = "A PulleyJoint.",
       type = "value"
      },
      revolute = {
       description = "A RevoluteJoint.",
       type = "value"
      },
      rope = {
       description = "A RopeJoint.",
       type = "value"
      },
      weld = {
       description = "A WeldJoint.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    MotorJoint = {
     childs = {
      getLinearOffset = {
       args = "()",
       description = "Gets the target linear offset between the two Bodies the Joint is attached to.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      setAngularOffset = {
       args = "(angleoffset: number)",
       description = "Sets the target angluar offset between the two Bodies the Joint is attached to.",
       returns = "()",
       type = "function"
      },
      setLinearOffset = {
       args = "(x: number, y: number)",
       description = "Sets the target linear offset between the two Bodies the Joint is attached to.",
       returns = "()",
       type = "function"
      }
     },
     description = "Controls the relative motion between two Bodies. Position and rotation offsets can be specified, as well as the maximum motor force and torque that will be applied to reach the target offsets.",
     inherits = "love.physics.Joint, love.Object",
     type = "class"
    },
    MouseJoint = {
     childs = {
      getFrequency = {
       args = "()",
       description = "Returns the frequency.",
       returns = "(freq: number)",
       type = "function"
      },
      getMaxForce = {
       args = "()",
       description = "Gets the highest allowed force.",
       returns = "(f: number)",
       type = "function"
      },
      getTarget = {
       args = "()",
       description = "Gets the target point.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      setDampingRatio = {
       args = "(ratio: number)",
       description = "Sets a new damping ratio.",
       returns = "()",
       type = "function"
      },
      setFrequency = {
       args = "(freq: number)",
       description = "Sets a new frequency.",
       returns = "()",
       type = "function"
      },
      setMaxForce = {
       args = "(f: number)",
       description = "Sets the highest allowed force.",
       returns = "()",
       type = "function"
      },
      setTarget = {
       args = "(x: number, y: number)",
       description = "Sets the target point.",
       returns = "()",
       type = "function"
      }
     },
     description = "For controlling objects with the mouse.",
     inherits = "love.physics.Joint, love.Object",
     type = "class"
    },
    PolygonShape = {
     childs = {
      getPoints = {
       args = "()",
       description = "Get the local coordinates of the polygon's vertices.\n\nThis function has a variable number of return values. It can be used in a nested fashion with love.graphics.polygon.",
       returns = "(x1: number, y1: number, x2: number, y2: number)",
       type = "function"
      }
     },
     description = "A PolygonShape is a convex polygon with up to 8 vertices.",
     inherits = "love.physics.Shape, love.Object",
     type = "class"
    },
    PrismaticJoint = {
     childs = {
      getAxis = {
       args = "()",
       description = "Gets the world-space axis vector of the Prismatic Joint.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      getJointSpeed = {
       args = "()",
       description = "Get the current joint angle speed.",
       returns = "(s: number)",
       type = "function"
      },
      getJointTranslation = {
       args = "()",
       description = "Get the current joint translation.",
       returns = "(t: number)",
       type = "function"
      },
      getLimits = {
       args = "()",
       description = "Gets the joint limits.",
       returns = "(lower: number, upper: number)",
       type = "function"
      },
      getLowerLimit = {
       args = "()",
       description = "Gets the lower limit.",
       returns = "(lower: number)",
       type = "function"
      },
      getMaxMotorForce = {
       args = "()",
       description = "Gets the maximum motor force.",
       returns = "(f: number)",
       type = "function"
      },
      getMotorForce = {
       args = "(invdt: number)",
       description = "Returns the current motor force.",
       returns = "(force: number)",
       type = "function"
      },
      getMotorSpeed = {
       args = "()",
       description = "Gets the motor speed.",
       returns = "(s: number)",
       type = "function"
      },
      getUpperLimit = {
       args = "()",
       description = "Gets the upper limit.",
       returns = "(upper: number)",
       type = "function"
      },
      isMotorEnabled = {
       args = "()",
       description = "Checks whether the motor is enabled.",
       returns = "(enabled: boolean)",
       type = "function"
      },
      setLimits = {
       args = "(lower: number, upper: number)",
       description = "Sets the limits.",
       returns = "()",
       type = "function"
      },
      setLimitsEnabled = {
       args = "()",
       description = "Enables/disables the joint limit.",
       returns = "(enable: boolean)",
       type = "function"
      },
      setLowerLimit = {
       args = "(lower: number)",
       description = "Sets the lower limit.",
       returns = "()",
       type = "function"
      },
      setMaxMotorForce = {
       args = "(f: number)",
       description = "Set the maximum motor force.",
       returns = "()",
       type = "function"
      },
      setMotorEnabled = {
       args = "(enable: boolean)",
       description = "Enables/disables the joint motor.",
       returns = "()",
       type = "function"
      },
      setMotorSpeed = {
       args = "(s: number)",
       description = "Sets the motor speed.",
       returns = "()",
       type = "function"
      },
      setUpperLimit = {
       args = "(upper: number)",
       description = "Sets the upper limit.",
       returns = "()",
       type = "function"
      }
     },
     description = "Restricts relative motion between Bodies to one shared axis.",
     inherits = "love.physics.Joint, love.Object",
     type = "class"
    },
    PulleyJoint = {
     childs = {
      getGroundAnchors = {
       args = "()",
       description = "Get the ground anchor positions in world coordinates.",
       returns = "(a1x: number, a1y: number, a2x: number, a2y: number)",
       type = "function"
      },
      getLengthA = {
       args = "()",
       description = "Get the current length of the rope segment attached to the first body.",
       returns = "(length: number)",
       type = "function"
      },
      getLengthB = {
       args = "()",
       description = "Get the current length of the rope segment attached to the second body.",
       returns = "(length: number)",
       type = "function"
      },
      getMaxLengths = {
       args = "()",
       description = "Get the maximum lengths of the rope segments.",
       returns = "(len1: number, len2: number)",
       type = "function"
      },
      getRatio = {
       args = "()",
       description = "Get the pulley ratio.",
       returns = "(ratio: number)",
       type = "function"
      },
      setConstant = {
       args = "(length: number)",
       description = "Set the total length of the rope.\n\nSetting a new length for the rope updates the maximum length values of the joint.",
       returns = "()",
       type = "function"
      },
      setMaxLengths = {
       args = "(max1: number, max2: number)",
       description = "Set the maximum lengths of the rope segments.\n\nThe physics module also imposes maximum values for the rope segments. If the parameters exceed these values, the maximum values are set instead of the requested values.",
       returns = "()",
       type = "function"
      },
      setRatio = {
       args = "(ratio: number)",
       description = "Set the pulley ratio.",
       returns = "()",
       type = "function"
      }
     },
     description = "Allows you to simulate bodies connected through pulleys.",
     inherits = "love.physics.Joint, love.Object",
     type = "class"
    },
    RevoluteJoint = {
     childs = {
      getJointAngle = {
       args = "()",
       description = "Get the current joint angle.",
       returns = "(angle: number)",
       type = "function"
      },
      getJointSpeed = {
       args = "()",
       description = "Get the current joint angle speed.",
       returns = "(s: number)",
       type = "function"
      },
      getLimits = {
       args = "()",
       description = "Gets the joint limits.",
       returns = "(lower: number, upper: number)",
       type = "function"
      },
      getLowerLimit = {
       args = "()",
       description = "Gets the lower limit.",
       returns = "(lower: number)",
       type = "function"
      },
      getMaxMotorTorque = {
       args = "()",
       description = "Gets the maximum motor force.",
       returns = "(f: number)",
       type = "function"
      },
      getMotorSpeed = {
       args = "()",
       description = "Gets the motor speed.",
       returns = "(s: number)",
       type = "function"
      },
      getMotorTorque = {
       args = "()",
       description = "Get the current motor force.",
       returns = "(f: number)",
       type = "function"
      },
      getUpperLimit = {
       args = "()",
       description = "Gets the upper limit.",
       returns = "(upper: number)",
       type = "function"
      },
      hasLimitsEnabled = {
       args = "()",
       description = "Checks whether limits are enabled.",
       returns = "(enabled: boolean)",
       type = "function"
      },
      isMotorEnabled = {
       args = "()",
       description = "Checks whether the motor is enabled.",
       returns = "(enabled: boolean)",
       type = "function"
      },
      setLimits = {
       args = "(lower: number, upper: number)",
       description = "Sets the limits.",
       returns = "()",
       type = "function"
      },
      setLimitsEnabled = {
       args = "(enable: boolean)",
       description = "Enables/disables the joint limit.",
       returns = "()",
       type = "function"
      },
      setLowerLimit = {
       args = "(lower: number)",
       description = "Sets the lower limit.",
       returns = "()",
       type = "function"
      },
      setMaxMotorTorque = {
       args = "(f: number)",
       description = "Set the maximum motor force.",
       returns = "()",
       type = "function"
      },
      setMotorEnabled = {
       args = "(enable: boolean)",
       description = "Enables/disables the joint motor.",
       returns = "()",
       type = "function"
      },
      setMotorSpeed = {
       args = "(s: number)",
       description = "Sets the motor speed.",
       returns = "()",
       type = "function"
      },
      setUpperLimit = {
       args = "(upper: number)",
       description = "Sets the upper limit.",
       returns = "()",
       type = "function"
      }
     },
     description = "Allow two Bodies to revolve around a shared point.",
     inherits = "love.physics.Joint, love.Object",
     type = "class"
    },
    RopeJoint = {
     childs = {
      setMaxLength = {
       args = "(maxLength: number)",
       description = "Sets the maximum length of a RopeJoint.",
       returns = "()",
       type = "function"
      }
     },
     description = "The RopeJoint enforces a maximum distance between two points on two bodies. It has no other effect.",
     inherits = "love.physics.Joint, love.Object",
     type = "class"
    },
    Shape = {
     childs = {
      computeMass = {
       args = "(density: number)",
       description = "Computes the mass properties for the shape with the specified density.",
       returns = "(x: number, y: number, mass: number, inertia: number)",
       type = "function"
      },
      getChildCount = {
       args = "()",
       description = "Returns the number of children the shape has.",
       returns = "(count: number)",
       type = "function"
      },
      getRadius = {
       args = "()",
       description = "Gets the radius of the shape.",
       returns = "(radius: number)",
       type = "function"
      },
      getType = {
       args = "()",
       description = "Gets a string representing the Shape.\n\nThis function can be useful for conditional debug drawing.",
       returns = "(type: ShapeType)",
       type = "function"
      },
      rayCast = {
       args = "(x1: number, y1: number, x2: number, y2: number, maxFraction: number, tx: number, ty: number, tr: number, childIndex: number)",
       description = "Casts a ray against the shape and returns the surface normal vector and the line position where the ray hit. If the ray missed the shape, nil will be returned. The Shape can be transformed to get it into the desired position.\n\nThe ray starts on the first point of the input line and goes towards the second point of the line. The fourth argument is the maximum distance the ray is going to travel as a scale factor of the input line length.\n\nThe childIndex parameter is used to specify which child of a parent shape, such as a ChainShape, will be ray casted. For ChainShapes, the index of 1 is the first edge on the chain. Ray casting a parent shape will only test the child specified so if you want to test every shape of the parent, you must loop through all of its children.\n\nThe world position of the impact can be calculated by multiplying the line vector with the third return value and adding it to the line starting point.\n\nhitx, hity = x1 + (x2 - x1) * fraction, y1 + (y2 - y1) * fraction",
       returns = "(xn: number, yn: number, fraction: number)",
       type = "function"
      },
      testPoint = {
       args = "(tx: number, ty: number, tr: number, x: number, y: number)",
       description = "This is particularly useful for mouse interaction with the shapes. By looping through all shapes and testing the mouse position with this function, we can find which shapes the mouse touches.",
       returns = "(hit: boolean)",
       type = "function"
      }
     },
     description = "Shapes are solid 2d geometrical objects which handle the mass and collision of a Body in love.physics.\n\nShapes are attached to a Body via a Fixture. The Shape object is copied when this happens. \n\nThe Shape's position is relative to the position of the Body it has been attached to.",
     inherits = "love.Object",
     type = "class"
    },
    ShapeType = {
     childs = {
      chain = {
       description = "The Shape is a ChainShape.",
       type = "value"
      },
      circle = {
       description = "The Shape is a CircleShape.",
       type = "value"
      },
      edge = {
       description = "The Shape is a EdgeShape.",
       type = "value"
      },
      polygon = {
       description = "The Shape is a PolygonShape.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    WeldJoint = {
     childs = {
      getFrequency = {
       args = "()",
       description = "Returns the frequency.",
       returns = "(freq: number)",
       type = "function"
      },
      setDampingRatio = {
       args = "(ratio: number)",
       description = "Sets a new damping ratio.",
       returns = "()",
       type = "function"
      },
      setFrequency = {
       args = "(freq: number)",
       description = "Sets a new frequency.",
       returns = "()",
       type = "function"
      }
     },
     description = "A WeldJoint essentially glues two bodies together.",
     inherits = "love.physics.Joint, love.Object",
     type = "class"
    },
    WheelJoint = {
     childs = {
      getJointSpeed = {
       args = "()",
       description = "Returns the current joint translation speed.",
       returns = "(speed: number)",
       type = "function"
      },
      getJointTranslation = {
       args = "()",
       description = "Returns the current joint translation.",
       returns = "(position: number)",
       type = "function"
      },
      getMaxMotorTorque = {
       args = "()",
       description = "Returns the maximum motor torque.",
       returns = "(maxTorque: number)",
       type = "function"
      },
      getMotorSpeed = {
       args = "()",
       description = "Returns the speed of the motor.",
       returns = "(speed: number)",
       type = "function"
      },
      getMotorTorque = {
       args = "(invdt: number)",
       description = "Returns the current torque on the motor.",
       returns = "(torque: number)",
       type = "function"
      },
      getSpringDampingRatio = {
       args = "()",
       description = "Returns the damping ratio.",
       returns = "(ratio: number)",
       type = "function"
      },
      getSpringFrequency = {
       args = "()",
       description = "Returns the spring frequency.",
       returns = "(freq: number)",
       type = "function"
      },
      setMaxMotorTorque = {
       args = "(maxTorque: number)",
       description = "Sets a new maximum motor torque.",
       returns = "()",
       type = "function"
      },
      setMotorEnabled = {
       args = "(enable: boolean)",
       description = "Starts and stops the joint motor.",
       returns = "()",
       type = "function"
      },
      setMotorSpeed = {
       args = "(speed: number)",
       description = "Sets a new speed for the motor.",
       returns = "()",
       type = "function"
      },
      setSpringDampingRatio = {
       args = "(ratio: number)",
       description = "Sets a new damping ratio.",
       returns = "()",
       type = "function"
      },
      setSpringFrequency = {
       args = "(freq: number)",
       description = "Sets a new spring frequency.",
       returns = "()",
       type = "function"
      }
     },
     description = "Restricts a point on the second body to a line on the first body.",
     inherits = "love.physics.Joint, love.Object",
     type = "class"
    },
    World = {
     childs = {
      getBodies = {
       args = "()",
       description = "Returns a table with all bodies.",
       returns = "(bodies: table)",
       type = "function"
      },
      getBodyCount = {
       args = "()",
       description = "Returns the number of bodies in the world.",
       returns = "(n: number)",
       type = "function"
      },
      getCallbacks = {
       args = "()",
       description = "Returns functions for the callbacks during the world update.",
       returns = "(beginContact: function, endContact: function, preSolve: function, postSolve: function)",
       type = "function"
      },
      getContactCount = {
       args = "()",
       description = "Returns the number of contacts in the world.",
       returns = "(n: number)",
       type = "function"
      },
      getContactFilter = {
       args = "()",
       description = "Returns the function for collision filtering.",
       returns = "(contactFilter: function)",
       type = "function"
      },
      getContacts = {
       args = "()",
       description = "Returns a table with all Contacts.",
       returns = "(contacts: table)",
       type = "function"
      },
      getGravity = {
       args = "()",
       description = "Get the gravity of the world.",
       returns = "(x: number, y: number)",
       type = "function"
      },
      getJointCount = {
       args = "()",
       description = "Returns the number of joints in the world.",
       returns = "(n: number)",
       type = "function"
      },
      getJoints = {
       args = "()",
       description = "Returns a table with all joints.",
       returns = "(joints: table)",
       type = "function"
      },
      isDestroyed = {
       args = "()",
       description = "Gets whether the World is destroyed. Destroyed worlds cannot be used.",
       returns = "(destroyed: boolean)",
       type = "function"
      },
      isLocked = {
       args = "()",
       description = "Returns if the world is updating its state.\n\nThis will return true inside the callbacks from World:setCallbacks.",
       returns = "(locked: boolean)",
       type = "function"
      },
      isSleepingAllowed = {
       args = "()",
       description = "Gets the sleep behaviour of the world.",
       returns = "(allow: boolean)",
       type = "function"
      },
      queryBoundingBox = {
       args = "(topLeftX: number, topLeftY: number, bottomRightX: number, bottomRightY: number, callback: function)",
       description = "Calls a function for each fixture inside the specified area by searching for any overlapping bounding box (Fixture:getBoundingBox).",
       returns = "()",
       type = "function"
      },
      rayCast = {
       args = "(fixture: Fixture, x: number, y: number, xn: number, yn: number, fraction: number)",
       description = "Casts a ray and calls a function for each fixtures it intersects. ",
       returns = "(control: number)",
       type = "function"
      },
      setCallbacks = {
       args = "(beginContact: function, endContact: function, preSolve: function, postSolve: function)",
       description = "Sets functions for the collision callbacks during the world update.\n\nFour Lua functions can be given as arguments. The value nil removes a function.\n\nWhen called, each function will be passed three arguments. The first two arguments are the colliding fixtures and the third argument is the Contact between them. The postSolve callback additionally gets the normal and tangent impulse for each contact point. See notes.\n\nIf you are interested to know when exactly each callback is called, consult a Box2d manual",
       returns = "()",
       type = "function"
      },
      setContactFilter = {
       args = "(filter: function)",
       description = "Sets a function for collision filtering.\n\nIf the group and category filtering doesn't generate a collision decision, this function gets called with the two fixtures as arguments. The function should return a boolean value where true means the fixtures will collide and false means they will pass through each other.",
       returns = "()",
       type = "function"
      },
      setGravity = {
       args = "(x: number, y: number)",
       description = "Set the gravity of the world.",
       returns = "()",
       type = "function"
      },
      setSleepingAllowed = {
       args = "(allow: boolean)",
       description = "Sets the sleep behaviour of the world.",
       returns = "()",
       type = "function"
      },
      translateOrigin = {
       args = "(x: number, y: number)",
       description = "Translates the World's origin. Useful in large worlds where floating point precision issues become noticeable at far distances from the origin.",
       returns = "()",
       type = "function"
      },
      update = {
       args = "(dt: number, velocityiterations: number, positioniterations: number)",
       description = "Update the state of the world.",
       returns = "()",
       type = "function"
      }
     },
     description = "A world is an object that contains all bodies and joints.",
     inherits = "love.Object",
     type = "class"
    },
    getMeter = {
     args = "()",
     description = "Returns the meter scale factor.\n\nAll coordinates in the physics module are divided by this number, creating a convenient way to draw the objects directly to the screen without the need for graphics transformations.\n\nIt is recommended to create shapes no larger than 10 times the scale. This is important because Box2D is tuned to work well with shape sizes from 0.1 to 10 meters.",
     returns = "(scale: number)",
     type = "function"
    },
    newBody = {
     args = "(world: World, x: number, y: number, type: BodyType)",
     description = "Creates a new body.\n\nThere are three types of bodies. \n\n* Static bodies do not move, have a infinite mass, and can be used for level boundaries. \n\n* Dynamic bodies are the main actors in the simulation, they collide with everything. \n\n* Kinematic bodies do not react to forces and only collide with dynamic bodies.\n\nThe mass of the body gets calculated when a Fixture is attached or removed, but can be changed at any time with Body:setMass or Body:resetMassData.",
     returns = "(body: Body)",
     type = "function",
     valuetype = "love.physics.Body"
    },
    newChainShape = {
     args = "(loop: boolean, x1: number, y1: number, x2: number, y2: number, ...: number)",
     description = "Creates a new ChainShape.",
     returns = "(shape: ChainShape)",
     type = "function",
     valuetype = "love.physics.ChainShape"
    },
    newCircleShape = {
     args = "(radius: number)",
     description = "Creates a new CircleShape.",
     returns = "(shape: CircleShape)",
     type = "function",
     valuetype = "love.physics.CircleShape"
    },
    newDistanceJoint = {
     args = "(body1: Body, body2: Body, x1: number, y1: number, x2: number, y2: number, collideConnected: boolean)",
     description = "Creates a DistanceJoint between two bodies.\n\nThis joint constrains the distance between two points on two bodies to be constant. These two points are specified in world coordinates and the two bodies are assumed to be in place when this joint is created. The first anchor point is connected to the first body and the second to the second body, and the points define the length of the distance joint.",
     returns = "(joint: DistanceJoint)",
     type = "function",
     valuetype = "love.physics.DistanceJoint"
    },
    newEdgeShape = {
     args = "(x1: number, y1: number, x2: number, y2: number)",
     description = "Creates a new EdgeShape.",
     returns = "(shape: EdgeShape)",
     type = "function",
     valuetype = "love.physics.EdgeShape"
    },
    newFixture = {
     args = "(body: Body, shape: Shape, density: number)",
     description = "Creates and attaches a Fixture to a body.\n\nNote that the Shape object is copied rather than kept as a reference when the Fixture is created. To get the Shape object that the Fixture owns, use Fixture:getShape.",
     returns = "(fixture: Fixture)",
     type = "function",
     valuetype = "love.physics.Fixture"
    },
    newFrictionJoint = {
     args = "(body1: Body, body2: Body, x: number, y: number, collideConnected: boolean)",
     description = "Create a friction joint between two bodies. A FrictionJoint applies friction to a body.",
     returns = "(joint: FrictionJoint)",
     type = "function",
     valuetype = "love.physics.FrictionJoint"
    },
    newGearJoint = {
     args = "(joint1: Joint, joint2: Joint, ratio: number, collideConnected: boolean)",
     description = "Create a GearJoint connecting two Joints.\n\nThe gear joint connects two joints that must be either  prismatic or  revolute joints. Using this joint requires that the joints it uses connect their respective bodies to the ground and have the ground as the first body. When destroying the bodies and joints you must make sure you destroy the gear joint before the other joints.\n\nThe gear joint has a ratio the determines how the angular or distance values of the connected joints relate to each other. The formula coordinate1 + ratio * coordinate2 always has a constant value that is set when the gear joint is created.",
     returns = "(joint: GearJoint)",
     type = "function",
     valuetype = "love.physics.GearJoint"
    },
    newMotorJoint = {
     args = "(body1: Body, body2: Body, correctionFactor: number)",
     description = "Creates a joint between two bodies which controls the relative motion between them.\n\nPosition and rotation offsets can be specified once the MotorJoint has been created, as well as the maximum motor force and torque that will be be applied to reach the target offsets.",
     returns = "(joint: MotorJoint)",
     type = "function",
     valuetype = "love.physics.MotorJoint"
    },
    newMouseJoint = {
     args = "(body: Body, x: number, y: number)",
     description = "Create a joint between a body and the mouse.\n\nThis joint actually connects the body to a fixed point in the world. To make it follow the mouse, the fixed point must be updated every timestep (example below).\n\nThe advantage of using a MouseJoint instead of just changing a body position directly is that collisions and reactions to other joints are handled by the physics engine. ",
     returns = "(joint: MouseJoint)",
     type = "function",
     valuetype = "love.physics.MouseJoint"
    },
    newPolygonShape = {
     args = "(x1: number, y1: number, x2: number, y2: number, x3: number, y3: number, ...: number)",
     description = "Creates a new PolygonShape.\n\nThis shape can have 8 vertices at most, and must form a convex shape.",
     returns = "(shape: PolygonShape)",
     type = "function",
     valuetype = "love.physics.PolygonShape"
    },
    newPrismaticJoint = {
     args = "(body1: Body, body2: Body, x: number, y: number, ax: number, ay: number, collideConnected: boolean)",
     description = "Creates a PrismaticJoint between two bodies.\n\nA prismatic joint constrains two bodies to move relatively to each other on a specified axis. It does not allow for relative rotation. Its definition and operation are similar to a  revolute joint, but with translation and force substituted for angle and torque.",
     returns = "(joint: PrismaticJoint)",
     type = "function",
     valuetype = "love.physics.PrismaticJoint"
    },
    newPulleyJoint = {
     args = "(body1: Body, body2: Body, gx1: number, gy1: number, gx2: number, gy2: number, x1: number, y1: number, x2: number, y2: number, ratio: number, collideConnected: boolean)",
     description = "Creates a PulleyJoint to join two bodies to each other and the ground.\n\nThe pulley joint simulates a pulley with an optional block and tackle. If the ratio parameter has a value different from one, then the simulated rope extends faster on one side than the other. In a pulley joint the total length of the simulated rope is the constant length1 + ratio * length2, which is set when the pulley joint is created.\n\nPulley joints can behave unpredictably if one side is fully extended. It is recommended that the method  setMaxLengths  be used to constrain the maximum lengths each side can attain.",
     returns = "(joint: PulleyJoint)",
     type = "function",
     valuetype = "love.physics.PulleyJoint"
    },
    newRectangleShape = {
     args = "(width: number, height: number)",
     description = "Shorthand for creating rectangular PolygonShapes. \n\nBy default, the local origin is located at the '''center''' of the rectangle as opposed to the top left for graphics.",
     returns = "(shape: PolygonShape)",
     type = "function",
     valuetype = "love.physics.PolygonShape"
    },
    newRevoluteJoint = {
     args = "(body1: Body, body2: Body, x: number, y: number, collideConnected: boolean)",
     description = "Creates a pivot joint between two bodies.\n\nThis joint connects two bodies to a point around which they can pivot.",
     returns = "(joint: RevoluteJoint)",
     type = "function",
     valuetype = "love.physics.RevoluteJoint"
    },
    newRopeJoint = {
     args = "(body1: Body, body2: Body, x1: number, y1: number, x2: number, y2: number, maxLength: number, collideConnected: boolean)",
     description = "Creates a joint between two bodies. Its only function is enforcing a max distance between these bodies.",
     returns = "(joint: RopeJoint)",
     type = "function",
     valuetype = "love.physics.RopeJoint"
    },
    newWeldJoint = {
     args = "(body1: Body, body2: Body, x: number, y: number, collideConnected: boolean)",
     description = "Creates a constraint joint between two bodies. A WeldJoint essentially glues two bodies together. The constraint is a bit soft, however, due to Box2D's iterative solver.",
     returns = "(joint: WeldJoint)",
     type = "function",
     valuetype = "love.physics.WeldJoint"
    },
    newWheelJoint = {
     args = "(body1: Body, body2: Body, x: number, y: number, ax: number, ay: number, collideConnected: boolean)",
     description = "Creates a wheel joint.",
     returns = "(joint: WheelJoint)",
     type = "function",
     valuetype = "love.physics.WheelJoint"
    },
    newWorld = {
     args = "(xg: number, yg: number, sleep: boolean)",
     description = "Creates a new World.",
     returns = "(world: World)",
     type = "function",
     valuetype = "love.physics.World"
    },
    setMeter = {
     args = "(scale: number)",
     description = "Sets the pixels to meter scale factor.\n\nAll coordinates in the physics module are divided by this number and converted to meters, and it creates a convenient way to draw the objects directly to the screen without the need for graphics transformations.\n\nIt is recommended to create shapes no larger than 10 times the scale. This is important because Box2D is tuned to work well with shape sizes from 0.1 to 10 meters. The default meter scale is 30.",
     returns = "()",
     type = "function"
    }
   },
   description = "Can simulate 2D rigid body physics in a realistic manner. This module is based on Box2D, and this API corresponds to the Box2D API as closely as possible.",
   type = "class"
  },
  quit = {
   args = "()",
   description = "Callback function triggered when the game is closed.",
   returns = "(r: boolean)",
   type = "function"
  },
  resize = {
   args = "(w: number, h: number)",
   description = "Called when the window is resized, for example if the user resizes the window, or if love.window.setMode is called with an unsupported width or height in fullscreen and the window chooses the closest appropriate size.",
   returns = "()",
   type = "function"
  },
  run = {
   args = "()",
   description = "The main function, containing the main loop. A sensible default is used when left out.",
   returns = "(mainLoop: function)",
   type = "function"
  },
  setDeprecationOutput = {
   args = "(enable: boolean)",
   description = "Sets whether LÖVE displays warnings when using deprecated functionality. It is disabled by default in fused mode, and enabled by default otherwise.\n\nWhen deprecation output is enabled, the first use of a formally deprecated LÖVE API will show a message at the bottom of the screen for a short time, and print the message to the console.",
   returns = "()",
   type = "function"
  },
  sound = {
   childs = {
    Decoder = {
     childs = {
      getBitDepth = {
       args = "()",
       description = "Returns the number of bits per sample.",
       returns = "(bitDepth: number)",
       type = "function"
      },
      getChannelCount = {
       args = "()",
       description = "Returns the number of channels in the stream.",
       returns = "(channels: number)",
       type = "function"
      },
      getDuration = {
       args = "()",
       description = "Gets the duration of the sound file. It may not always be sample-accurate, and it may return -1 if the duration cannot be determined at all.",
       returns = "(duration: number)",
       type = "function"
      },
      getSampleRate = {
       args = "()",
       description = "Returns the sample rate of the Decoder.",
       returns = "(rate: number)",
       type = "function"
      }
     },
     description = "An object which can gradually decode a sound file.",
     inherits = "love.Object",
     type = "class"
    },
    SoundData = {
     childs = {
      getChannelCount = {
       args = "()",
       description = "Returns the number of channels in the SoundData.",
       returns = "(channels: number)",
       type = "function"
      },
      getDuration = {
       args = "()",
       description = "Gets the duration of the sound data.",
       returns = "(duration: number)",
       type = "function"
      },
      getSample = {
       args = "(i: number)",
       description = "Gets the value of the sample-point at the specified position. For stereo SoundData objects, the data from the left and right channels are interleaved in that order.",
       returns = "(sample: number)",
       type = "function"
      },
      getSampleCount = {
       args = "()",
       description = "Returns the number of samples per channel of the SoundData.",
       returns = "(count: number)",
       type = "function"
      },
      getSampleRate = {
       args = "()",
       description = "Returns the sample rate of the SoundData.",
       returns = "(rate: number)",
       type = "function"
      },
      setSample = {
       args = "(i: number, sample: number)",
       description = "Sets the value of the sample-point at the specified position. For stereo SoundData objects, the data from the left and right channels are interleaved in that order.",
       returns = "()",
       type = "function"
      }
     },
     description = "Contains raw audio samples.\n\nYou can not play SoundData back directly. You must wrap a Source object around it.",
     inherits = "love.Data, love.Object",
     type = "class"
    },
    newSoundData = {
     args = "(filename: string)",
     description = "Creates new SoundData from a filepath, File, or Decoder. It's also possible to create SoundData with a custom sample rate, channel and bit depth.\n\nThe sound data will be decoded to the memory in a raw format. It is recommended to create only short sounds like effects, as a 3 minute song uses 30 MB of memory this way.",
     returns = "(soundData: SoundData)",
     type = "function",
     valuetype = "love.sound.SoundData"
    }
   },
   description = "This module is responsible for decoding sound files. It can't play the sounds, see love.audio for that.",
   type = "class"
  },
  system = {
   childs = {
    PowerState = {
     childs = {
      battery = {
       description = "Not plugged in, running on a battery.",
       type = "value"
      },
      charged = {
       description = "Plugged in, battery is fully charged.",
       type = "value"
      },
      charging = {
       description = "Plugged in, charging battery.",
       type = "value"
      },
      nobattery = {
       description = "Plugged in, no battery available.",
       type = "value"
      },
      unknown = {
       description = "Cannot determine power status.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    getOS = {
     args = "()",
     description = "Gets the current operating system. In general, LÖVE abstracts away the need to know the current operating system, but there are a few cases where it can be useful (especially in combination with os.execute.)",
     returns = "(osString: string)",
     type = "function"
    },
    getPowerInfo = {
     args = "()",
     description = "Gets information about the system's power supply.",
     returns = "(state: PowerState, percent: number, seconds: number)",
     type = "function"
    },
    getProcessorCount = {
     args = "()",
     description = "Gets the amount of logical processor in the system.",
     returns = "(processorCount: number)",
     type = "function"
    },
    hasBackgroundMusic = {
     args = "()",
     description = "Gets whether another application on the system is playing music in the background.\n\nCurrently this is implemented on iOS and Android, and will always return false on other operating systems. The t.audio.mixwithsystem flag in love.conf can be used to configure whether background audio / music from other apps should play while LÖVE is open.",
     returns = "(backgroundmusic: boolean)",
     type = "function"
    },
    openURL = {
     args = "(url: string)",
     description = "Opens a URL with the user's web or file browser.",
     returns = "(success: boolean)",
     type = "function"
    },
    setClipboardText = {
     args = "(text: string)",
     description = "Puts text in the clipboard.",
     returns = "()",
     type = "function"
    },
    vibrate = {
     args = "(seconds: number)",
     description = "Causes the device to vibrate, if possible. Currently this will only work on Android and iOS devices that have a built-in vibration motor.",
     returns = "()",
     type = "function"
    }
   },
   description = "Provides access to information about the user's system.",
   type = "class"
  },
  textedited = {
   args = "(text: string, start: number, length: number)",
   description = "Called when the candidate text for an IME (Input Method Editor) has changed.\n\nThe candidate text is not the final text that the user will eventually choose. Use love.textinput for that.",
   returns = "()",
   type = "function"
  },
  textinput = {
   args = "(text: string)",
   description = "Called when text has been entered by the user. For example if shift-2 is pressed on an American keyboard layout, the text '@' will be generated.",
   returns = "()",
   type = "function"
  },
  thread = {
   childs = {
    Channel = {
     childs = {
      demand = {
       args = "(timeout: number)",
       description = "Retrieves the value of a Channel message and removes it from the message queue.\n\nIt waits until a message is in the queue then returns the message value.",
       returns = "(value: Variant)",
       type = "function"
      },
      getCount = {
       args = "()",
       description = "Retrieves the number of messages in the thread Channel queue.",
       returns = "(count: number)",
       type = "function"
      },
      hasRead = {
       args = "(id: number)",
       description = "Gets whether a pushed value has been popped or otherwise removed from the Channel.",
       returns = "(hasread: boolean)",
       type = "function"
      },
      peek = {
       args = "()",
       description = "Retrieves the value of a Channel message, but leaves it in the queue.\n\nIt returns nil if there's no message in the queue.",
       returns = "(value: Variant)",
       type = "function"
      },
      performAtomic = {
       args = "(func: function, arg1: any, ...: any)",
       description = "Executes the specified function atomically with respect to this Channel.\n\nCalling multiple methods in a row on the same Channel is often useful. However if multiple Threads are calling this Channel's methods at the same time, the different calls on each Thread might end up interleaved (e.g. one or more of the second thread's calls may happen in between the first thread's calls.)\n\nThis method avoids that issue by making sure the Thread calling the method has exclusive access to the Channel until the specified function has returned.",
       returns = "(ret1: any, ...: any)",
       type = "function"
      },
      pop = {
       args = "()",
       description = "Retrieves the value of a Channel message and removes it from the message queue.\n\nIt returns nil if there are no messages in the queue.",
       returns = "(value: Variant)",
       type = "function"
      },
      push = {
       args = "(value: Variant)",
       description = "Send a message to the thread Channel.\n\nSee Variant for the list of supported types.",
       returns = "(id: number)",
       type = "function"
      },
      supply = {
       args = "(value: Variant)",
       description = "Send a message to the thread Channel and wait for a thread to accept it.\n\nSee Variant for the list of supported types.",
       returns = "(success: boolean)",
       type = "function"
      }
     },
     description = "An object which can be used to send and receive data between different threads.",
     inherits = "love.Object",
     type = "class"
    },
    Thread = {
     childs = {
      isRunning = {
       args = "()",
       description = "Returns whether the thread is currently running.\n\nThreads which are not running can be (re)started with Thread:start.",
       returns = "(value: boolean)",
       type = "function"
      },
      start = {
       args = "(arg1: Variant, arg2: Variant, ...: Variant)",
       description = "Starts the thread.\n\nBeginning with version 0.9.0, threads can be restarted after they have completed their execution.",
       returns = "()",
       type = "function"
      },
      wait = {
       args = "()",
       description = "Wait for a thread to finish.\n\nThis call will block until the thread finishes.",
       returns = "()",
       type = "function"
      }
     },
     description = "A Thread is a chunk of code that can run in parallel with other threads. Data can be sent between different threads with Channel objects.",
     inherits = "love.Object",
     type = "class"
    },
    newChannel = {
     args = "()",
     description = "Create a new unnamed thread channel.\n\nOne use for them is to pass new unnamed channels to other threads via Channel:push on a named channel.",
     returns = "(channel: Channel)",
     type = "function",
     valuetype = "love.thread.Channel"
    },
    newThread = {
     args = "(filename: string)",
     description = "Creates a new Thread from a filename, string or FileData object containing Lua code.",
     returns = "(thread: Thread)",
     type = "function",
     valuetype = "love.thread.Thread"
    }
   },
   description = "Allows you to work with threads.\n\nThreads are separate Lua environments, running in parallel to the main code. As their code runs separately, they can be used to compute complex operations without adversely affecting the frame rate of the main thread. However, as they are separate environments, they cannot access the variables and functions of the main thread, and communication between threads is limited.\n\nAll LOVE objects (userdata) are shared among threads so you'll only have to send their references across threads. You may run into concurrency issues if you manipulate an object on multiple threads at the same time.\n\nWhen a Thread is started, it only loads the love.thread module. Every other module has to be loaded with require.",
   type = "class"
  },
  threaderror = {
   args = "(thread: Thread, errorstr: string)",
   description = "Callback function triggered when a Thread encounters an error.",
   returns = "()",
   type = "function"
  },
  timer = {
   childs = {
    getDelta = {
     args = "()",
     description = "Returns the time between the last two frames.",
     returns = "(dt: number)",
     type = "function"
    },
    getFPS = {
     args = "()",
     description = "Returns the current frames per second.",
     returns = "(fps: number)",
     type = "function"
    },
    getTime = {
     args = "()",
     description = "Returns the value of a timer with an unspecified starting time.\n\nThis function should only be used to calculate differences between points in time, as the starting time of the timer is unknown.",
     returns = "(time: number)",
     type = "function"
    },
    sleep = {
     args = "(s: number)",
     description = "Pauses the current thread for the specified amount of time.",
     returns = "()",
     type = "function"
    },
    step = {
     args = "()",
     description = "Measures the time between two frames.\n\nCalling this changes the return value of love.timer.getDelta.",
     returns = "(dt: number)",
     type = "function"
    }
   },
   description = "Provides an interface to the user's clock.",
   type = "class"
  },
  touch = {
   childs = {
    getPressure = {
     args = "(id: light userdata)",
     description = "Gets the current pressure of the specified touch-press.",
     returns = "(pressure: number)",
     type = "function"
    },
    getTouches = {
     args = "()",
     description = "Gets a list of all active touch-presses.",
     returns = "(touches: table)",
     type = "function"
    }
   },
   description = "Provides an interface to touch-screen presses.",
   type = "class"
  },
  touchmoved = {
   args = "(id: light userdata, x: number, y: number, dx: number, dy: number, pressure: number)",
   description = "Callback function triggered when a touch press moves inside the touch screen.",
   returns = "()",
   type = "function"
  },
  touchpressed = {
   args = "(id: light userdata, x: number, y: number, dx: number, dy: number, pressure: number)",
   description = "Callback function triggered when the touch screen is touched.",
   returns = "()",
   type = "function"
  },
  touchreleased = {
   args = "(id: light userdata, x: number, y: number, dx: number, dy: number, pressure: number)",
   description = "Callback function triggered when the touch screen stops being touched.",
   returns = "()",
   type = "function"
  },
  update = {
   args = "(dt: number)",
   description = "Callback function used to update the state of the game every frame.",
   returns = "()",
   type = "function"
  },
  video = {
   childs = {
    VideoStream = {
     description = "An object which decodes, streams, and controls Videos.",
     inherits = "love.Object",
     type = "class"
    },
    newVideoStream = {
     args = "(filename: string)",
     description = "Creates a new VideoStream. Currently only Ogg Theora video files are supported. VideoStreams can't draw videos, see love.graphics.newVideo for that.",
     returns = "(videostream: VideoStream)",
     type = "function",
     valuetype = "love.video.VideoStream"
    }
   },
   description = "This module is responsible for decoding, controlling, and streaming video files.\n\nIt can't draw the videos, see love.graphics.newVideo and Video objects for that.",
   type = "class"
  },
  visible = {
   args = "(visible: boolean)",
   description = "Callback function triggered when window is minimized/hidden or unminimized by the user.",
   returns = "()",
   type = "function"
  },
  wheelmoved = {
   args = "(x: number, y: number)",
   description = "Callback function triggered when the mouse wheel is moved.",
   returns = "()",
   type = "function"
  },
  window = {
   childs = {
    FullscreenType = {
     childs = {
      desktop = {
       description = "Sometimes known as borderless fullscreen windowed mode. A borderless screen-sized window is created which sits on top of all desktop UI elements. The window is automatically resized to match the dimensions of the desktop, and its size cannot be changed.",
       type = "value"
      },
      exclusive = {
       description = "Standard exclusive-fullscreen mode. Changes the display mode (actual resolution) of the monitor.",
       type = "value"
      },
      normal = {
       description = "Standard exclusive-fullscreen mode. Changes the display mode (actual resolution) of the monitor.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    MessageBoxType = {
     childs = {
      error = {
       description = "Error dialog.",
       type = "value"
      },
      info = {
       description = "Informational dialog.",
       type = "value"
      },
      warning = {
       description = "Warning dialog.",
       type = "value"
      }
     },
     description = "class constants",
     type = "class"
    },
    fromPixels = {
     args = "(pixelvalue: number)",
     description = "Converts a number from pixels to density-independent units.\n\nThe pixel density inside the window might be greater (or smaller) than the 'size' of the window. For example on a retina screen in Mac OS X with the highdpi window flag enabled, the window may take up the same physical size as an 800x600 window, but the area inside the window uses 1600x1200 pixels. love.window.fromPixels(1600) would return 800 in that case.\n\nThis function converts coordinates from pixels to the size users are expecting them to display at onscreen. love.window.toPixels does the opposite. The highdpi window flag must be enabled to use the full pixel density of a Retina screen on Mac OS X and iOS. The flag currently does nothing on Windows and Linux, and on Android it is effectively always enabled.\n\nMost LÖVE functions return values and expect arguments in terms of pixels rather than density-independent units.",
     returns = "(value: number)",
     type = "function"
    },
    getDPIScale = {
     args = "()",
     description = "Gets the DPI scale factor associated with the window.\n\nThe pixel density inside the window might be greater (or smaller) than the 'size' of the window. For example on a retina screen in Mac OS X with the highdpi window flag enabled, the window may take up the same physical size as an 800x600 window, but the area inside the window uses 1600x1200 pixels. love.window.getDPIScale() would return 2.0 in that case.\n\nThe love.window.fromPixels and love.window.toPixels functions can also be used to convert between units.\n\nThe highdpi window flag must be enabled to use the full pixel density of a Retina screen on Mac OS X and iOS. The flag currently does nothing on Windows and Linux, and on Android it is effectively always enabled.",
     returns = "(scale: number)",
     type = "function"
    },
    getDisplayName = {
     args = "(displayindex: number)",
     description = "Gets the name of a display.",
     returns = "(name: string)",
     type = "function"
    },
    getDisplayOrientation = {
     args = "(index: number)",
     description = "Gets current device display orientation.",
     returns = "(orientation: DisplayOrientation)",
     type = "function"
    },
    getFullscreen = {
     args = "()",
     description = "Gets whether the window is fullscreen.",
     returns = "(fullscreen: boolean, fstype: FullscreenType)",
     type = "function"
    },
    getFullscreenModes = {
     args = "(display: number)",
     description = "Gets a list of supported fullscreen modes.",
     returns = "(modes: table)",
     type = "function"
    },
    getIcon = {
     args = "()",
     description = "Gets the window icon.",
     returns = "(imagedata: ImageData)",
     type = "function",
     valuetype = "love.image.ImageData"
    },
    getMode = {
     args = "()",
     description = "Gets the display mode and properties of the window.",
     returns = "(width: number, height: number, flags: table)",
     type = "function"
    },
    getPosition = {
     args = "()",
     description = "Gets the position of the window on the screen.\n\nThe window position is in the coordinate space of the display it is currently in.",
     returns = "(x: number, y: number, display: number)",
     type = "function"
    },
    getSafeArea = {
     args = "()",
     description = "Gets area inside the window which is known to be unobstructed by a system title bar, the iPhone X notch, etc. Useful for making sure UI elements can be seen by the user.",
     returns = "(x: number, y: number, w: number, h: number)",
     type = "function"
    },
    getTitle = {
     args = "()",
     description = "Gets the window title.",
     returns = "(title: string)",
     type = "function"
    },
    getVSync = {
     args = "()",
     description = "Gets current vertical synchronization (vsync).",
     returns = "(vsync: number)",
     type = "function"
    },
    hasFocus = {
     args = "()",
     description = "Checks if the game window has keyboard focus.",
     returns = "(focus: boolean)",
     type = "function"
    },
    hasMouseFocus = {
     args = "()",
     description = "Checks if the game window has mouse focus.",
     returns = "(focus: boolean)",
     type = "function"
    },
    isDisplaySleepEnabled = {
     args = "()",
     description = "Gets whether the display is allowed to sleep while the program is running.\n\nDisplay sleep is disabled by default. Some types of input (e.g. joystick button presses) might not prevent the display from sleeping, if display sleep is allowed.",
     returns = "(enabled: boolean)",
     type = "function"
    },
    isMaximized = {
     args = "()",
     description = "Gets whether the Window is currently maximized.\n\nThe window can be maximized if it is not fullscreen and is resizable, and either the user has pressed the window's Maximize button or love.window.maximize has been called.",
     returns = "(maximized: boolean)",
     type = "function"
    },
    isMinimized = {
     args = "()",
     description = "Gets whether the Window is currently minimized.",
     returns = "(minimized: boolean)",
     type = "function"
    },
    isOpen = {
     args = "()",
     description = "Checks if the window is open.",
     returns = "(open: boolean)",
     type = "function"
    },
    isVisible = {
     args = "()",
     description = "Checks if the game window is visible.\n\nThe window is considered visible if it's not minimized and the program isn't hidden.",
     returns = "(visible: boolean)",
     type = "function"
    },
    maximize = {
     args = "()",
     description = "Makes the window as large as possible.\n\nThis function has no effect if the window isn't resizable, since it essentially programmatically presses the window's 'maximize' button.",
     returns = "()",
     type = "function"
    },
    minimize = {
     args = "()",
     description = "Minimizes the window to the system's task bar / dock.",
     returns = "()",
     type = "function"
    },
    requestAttention = {
     args = "(continuous: boolean)",
     description = "Causes the window to request the attention of the user if it is not in the foreground.\n\nIn Windows the taskbar icon will flash, and in OS X the dock icon will bounce.",
     returns = "()",
     type = "function"
    },
    restore = {
     args = "()",
     description = "Restores the size and position of the window if it was minimized or maximized.",
     returns = "()",
     type = "function"
    },
    setDisplaySleepEnabled = {
     args = "(enable: boolean)",
     description = "Sets whether the display is allowed to sleep while the program is running.\n\nDisplay sleep is disabled by default. Some types of input (e.g. joystick button presses) might not prevent the display from sleeping, if display sleep is allowed.",
     returns = "()",
     type = "function"
    },
    setFullscreen = {
     args = "(fullscreen: boolean)",
     description = "Enters or exits fullscreen. The display to use when entering fullscreen is chosen based on which display the window is currently in, if multiple monitors are connected.",
     returns = "(success: boolean)",
     type = "function"
    },
    setIcon = {
     args = "(imagedata: ImageData)",
     description = "Sets the window icon until the game is quit. Not all operating systems support very large icon images.",
     returns = "(success: boolean)",
     type = "function"
    },
    setMode = {
     args = "(width: number, height: number, flags: table)",
     description = "Sets the display mode and properties of the window.\n\nIf width or height is 0, setMode will use the width and height of the desktop. \n\nChanging the display mode may have side effects: for example, canvases will be cleared and values sent to shaders with canvases beforehand or re-draw to them afterward if you need to.",
     returns = "(success: boolean)",
     type = "function"
    },
    setPosition = {
     args = "(x: number, y: number, display: number)",
     description = "Sets the position of the window on the screen.\n\nThe window position is in the coordinate space of the specified display.",
     returns = "()",
     type = "function"
    },
    setTitle = {
     args = "(title: string)",
     description = "Sets the window title.",
     returns = "()",
     type = "function"
    },
    setVSync = {
     args = "(vsync: number)",
     description = "Sets vertical synchronization mode.",
     returns = "()",
     type = "function"
    },
    showMessageBox = {
     args = "(title: string, message: string, type: MessageBoxType, attachtowindow: boolean)",
     description = "Displays a message box dialog above the love window. The message box contains a title, optional text, and buttons.",
     returns = "(success: boolean)",
     type = "function"
    },
    toPixels = {
     args = "(value: number)",
     description = "Converts a number from density-independent units to pixels.\n\nThe pixel density inside the window might be greater (or smaller) than the 'size' of the window. For example on a retina screen in Mac OS X with the highdpi window flag enabled, the window may take up the same physical size as an 800x600 window, but the area inside the window uses 1600x1200 pixels. love.window.toPixels(800) would return 1600 in that case.\n\nThis is used to convert coordinates from the size users are expecting them to display at onscreen to pixels. love.window.fromPixels does the opposite. The highdpi window flag must be enabled to use the full pixel density of a Retina screen on Mac OS X and iOS. The flag currently does nothing on Windows and Linux, and on Android it is effectively always enabled.\n\nMost LÖVE functions return values and expect arguments in terms of pixels rather than density-independent units.",
     returns = "(pixelvalue: number)",
     type = "function"
    },
    updateMode = {
     args = "(width: number, height: number, settings: table)",
     description = "Sets the display mode and properties of the window, without modifying unspecified properties.\n\nIf width or height is 0, updateMode will use the width and height of the desktop. \n\nChanging the display mode may have side effects: for example, canvases will be cleared. Make sure to save the contents of canvases beforehand or re-draw to them afterward if you need to.",
     returns = "(success: boolean)",
     type = "function"
    }
   },
   description = "Provides an interface for modifying and retrieving information about the program's window.",
   type = "class"
  }
 },
 description = "Love2d modules, functions, and callbacks.",
 type = "lib",
 version = "11.3"
}

-- when loaded as a package, return the package; otherwise continue with the script
if pcall(debug.getlocal, 4, 1) then return {love = love} end

-- the following code is used to convert love_api.lua to a proper format
love = require('love_api')

-- conversion script
local classmap = {}
local function remap(l, pname)
  pname = pname or "love"
  for _,v in ipairs(l.types or {}) do
    classmap[v.name] = pname.."."..v.name
  end
  for _,v in ipairs(l.modules or {}) do
    remap(v, pname.."."..v.name)
  end
end
local function convert(l)
  local function merge(...) -- merges tables into one table
    local r = {}
    for _,v in pairs({...}) do
      for _,e in pairs(v) do table.insert(r, e) end
    end
    return r
  end
  local function params(t) -- merges parameters and return results
    if not t then return end
    local r = {}
    for _,v in ipairs(t) do
      table.insert(r, v.name .. ': ' .. v.type)
    end
    return '(' .. table.concat(r, ", ") .. ')'
  end
  local function expand(v)
    -- table or string
    if type(v) ~= "table" then
      return classmap[v]
    end
    local t = {}
    for _, val in ipairs(v) do
      if classmap[val] then table.insert(t, classmap[val]) end
    end
    return t
  end

  if l.modules then
    l.description = 'Love2d modules, functions, and callbacks.'
    l.type = "lib"
    l.childs = merge(l.modules, l.functions or {}, l.callbacks or {}, l.types or {})
    l.types = nil
    l.callbacks = nil
    l.functions = nil
    l.modules = nil
    l.subtypes = nil
  end

  if not l.childs then return end

  for n,v in ipairs(l.childs) do
    if v.functions and #v.functions > 1 and #v.functions[1] == 0 then
      io.stderr:write("Alternative signature ignored for "..v.name..".\n")
      table.remove(v.functions, 1)
    end
    v.childs = merge(v.types, v.functions, v.constants, v.enums)
    if v.name then
      l.childs[v.name] = v
      v.name = nil
    end
    if #v.childs > 0 and v.childs[1] then
      if v.childs[1].returns then v.returns = params(v.childs[1].returns) end
      if v.childs[1].arguments then v.args = params(v.childs[1].arguments) end
    end
    -- some nodes have first chils as empty and the data is in the second one (Mouse.setCursor)
    if v.variants and #v.variants > 0 then
      v.returns = params(v.variants[1] and v.variants[1].returns or v.variants[2] and v.variants[2].returns)
    end
    if v.variants and #v.variants > 0 then
      v.args = params(v.variants[1] and v.variants[1].arguments or v.variants[2] and v.variants[2].arguments)
    end
    local nochildren = #v.childs == 0 or v.returns or v.args
    v.type = nochildren and ((v.returns or v.args or v.variants) and "function" or v.functions and "class" or "value")
      or v.types and "class"
      or v.functions and "lib"
      or v.constants and "class"
      or "function"
    if v.constants then v.description = "class constants" end
    v.variants = nil
    v.types = nil
    v.functions = nil
    v.constants = nil
    v.enums = nil
    v.constructors = nil
    v.subtypes = nil
    if nochildren then v.childs = nil end
    if v.type == "function" then
      v.args = v.args or '()'
      v.returns = v.returns or '()'
    end
    if v.returns then
      -- assign valuetype value to `Cursor` if it matches `(something: Cursor)` format
      v.valuetype = expand(v.returns:match(": ([A-Z]%w+)%)"))
    end
    if v.parenttype or v.subtypes or v.supertypes then
      v.inherits = v.parenttype or v.supertypes and table.concat(expand(v.supertypes), ", ")
      v.type = "class"
    end
    v.supertypes = nil
    v.parenttype = nil
    l.childs[n] = nil
    convert(v)
  end
  return l
end

package.path = package.path .. ';../../lualibs/?/?.lua;../../lualibs/?.lua'
package.cpath = package.cpath .. ';../../bin/clibs/?.dll'
remap(love) -- create mapping from short class name to full one
print((require 'mobdebug').line(convert(love), {indent = ' ', comment = false}))
