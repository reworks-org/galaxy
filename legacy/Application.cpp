config.restore<std::string>("materials_folder", "materials/", "resource_folders");
create_asset_layout(root, config.get<std::string>("materials_folder", "resource_folders"));