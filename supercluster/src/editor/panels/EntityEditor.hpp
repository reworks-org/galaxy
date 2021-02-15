/*
///
/// EntityEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_

namespace sc
{
	namespace panel
	{
		///
		/// Parses EntityEditor and provides imgui widgets to manipulate it.
		///
		class EntityEditor final
		{
		public:
			void pre_render();
			void render();

		private:
			robin_hood::unordered_map<components::Sprite*, std::string> m_sprites_to_create;
			bool m_show_entity_create;
			std::string m_edn_buffer;
			std::string m_entity_debug_name;
			std::uint64_t m_active_entity;

			float m_batch_region[4]   = {0.0f, 0.0f, 0.0f, 0.0f};
			bool m_sfx_loop           = false;
			std::string m_active_anim = "";
			bool m_add_anim_popup     = false;
		};
	} // namespace panel
} // namespace sc

#endif
*/