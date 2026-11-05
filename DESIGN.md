App -> Loads Engine, Resources, FS, etc.

fs -> loads a .galaxy (zip) file, containing base.json + asset folders or just a raw directory if use_loose_assets
call load() -> reads the base.json from the zip.
