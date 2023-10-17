///
/// LuaNuklear.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <Nuklear.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_nuklear()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			// clang-format off
			lua.new_enum<nk_heading>("nk_heading",
			{
				{"NK_UP", NK_UP},
				{"NK_RIGHT", NK_RIGHT},
				{"NK_DOWN", NK_DOWN},
				{"NK_LEFT",NK_LEFT}
			});

			lua.new_enum<enum nk_button_behavior>("nk_button_behavior",
			{
				{"NK_BUTTON_DEFAULT", NK_BUTTON_DEFAULT},
				{"NK_BUTTON_REPEATER", NK_BUTTON_REPEATER}
			});

			lua.new_enum<nk_modify>("nk_modify",
			{
				{"NK_FIXED", NK_FIXED},
				{"NK_MODIFIABLE", NK_MODIFIABLE}
			});

			lua.new_enum<nk_orientation>("nk_orientation",
			{
				{"NK_VERTICAL", NK_VERTICAL},
				{"NK_HORIZONTAL", NK_HORIZONTAL}
			});

			lua.new_enum<nk_collapse_states>("nk_collapse_states",
			{
				{"NK_MINIMIZED", NK_MINIMIZED},
				{"NK_MAXIMIZED", NK_MAXIMIZED}
			});

			lua.new_enum<nk_show_states>("nk_show_states",
			{
				{"NK_HIDDEN", NK_HIDDEN},
				{"NK_SHOWN", NK_SHOWN}
			});

			lua.new_enum<nk_chart_type>("nk_chart_type",
			{
				{"NK_CHART_LINES", NK_CHART_LINES},
				{"NK_CHART_COLUMN", NK_CHART_COLUMN},
				{"NK_CHART_MAX", NK_CHART_MAX}
			});

			lua.new_enum<nk_chart_event>("nk_chart_event",
			{
				{"NK_CHART_HOVERING", NK_CHART_HOVERING},
				{"NK_CHART_CLICKED", NK_CHART_CLICKED}
			});

			lua.new_enum<nk_color_format>("nk_color_format",
			{
				{"NK_RGB", NK_RGB},
				{"NK_RGBA", NK_RGBA}
			});

			lua.new_enum<nk_popup_type>("nk_popup_type",
			{
				{"NK_POPUP_STATIC", NK_POPUP_STATIC},
				{"NK_POPUP_DYNAMIC", NK_POPUP_DYNAMIC}
			});

			lua.new_enum<nk_layout_format>("nk_layout_format",
			{
				{"NK_DYNAMIC", NK_DYNAMIC},
				{"NK_STATIC", NK_STATIC}
			});

			lua.new_enum<nk_tree_type>("nk_tree_type",
			{
				{"NK_TREE_NODE", NK_TREE_NODE},
				{"NK_TREE_TAB", NK_TREE_TAB}
			});

			lua.new_enum<nk_symbol_type>("nk_symbol_type",
			{
				{"NK_SYMBOL_NONE", NK_SYMBOL_NONE},
				{"NK_SYMBOL_X", NK_SYMBOL_X},
				{"NK_SYMBOL_UNDERSCORE", NK_SYMBOL_UNDERSCORE},
				{"NK_SYMBOL_CIRCLE_SOLID", NK_SYMBOL_CIRCLE_SOLID},
				{"NK_SYMBOL_CIRCLE_OUTLINE", NK_SYMBOL_CIRCLE_OUTLINE},
				{"NK_SYMBOL_RECT_SOLID", NK_SYMBOL_RECT_SOLID},
				{"NK_SYMBOL_RECT_OUTLINE", NK_SYMBOL_RECT_OUTLINE},
				{"NK_SYMBOL_TRIANGLE_UP", NK_SYMBOL_TRIANGLE_UP},
				{"NK_SYMBOL_TRIANGLE_DOWN", NK_SYMBOL_TRIANGLE_DOWN},
				{"NK_SYMBOL_TRIANGLE_LEFT", NK_SYMBOL_TRIANGLE_LEFT},
				{"NK_SYMBOL_TRIANGLE_RIGHT", NK_SYMBOL_TRIANGLE_RIGHT},
				{"NK_SYMBOL_PLUS", NK_SYMBOL_PLUS},
				{"NK_SYMBOL_MINUS", NK_SYMBOL_MINUS},
				{"NK_SYMBOL_MAX", NK_SYMBOL_MAX}
			});

			lua.new_enum<nk_keys>("nk_keys",
			{
				{"NK_KEY_NONE", NK_KEY_NONE},
				{"NK_KEY_SHIFT", NK_KEY_SHIFT},
				{"NK_KEY_CTRL", NK_KEY_CTRL},
				{"NK_KEY_DEL", NK_KEY_DEL},
				{"NK_KEY_ENTER", NK_KEY_ENTER},
				{"NK_KEY_TAB", NK_KEY_TAB},
				{"NK_KEY_BACKSPACE", NK_KEY_BACKSPACE},
				{"NK_KEY_COPY", NK_KEY_COPY},
				{"NK_KEY_CUT", NK_KEY_CUT},
				{"NK_KEY_PASTE", NK_KEY_PASTE},
				{"NK_KEY_UP", NK_KEY_UP},
				{"NK_KEY_DOWN", NK_KEY_DOWN},
				{"NK_KEY_LEFT", NK_KEY_LEFT},
				{"NK_KEY_RIGHT", NK_KEY_RIGHT},
				{"NK_KEY_TEXT_INSERT_MODE", NK_KEY_TEXT_INSERT_MODE},
				{"NK_KEY_TEXT_REPLACE_MODE", NK_KEY_TEXT_REPLACE_MODE},
				{"NK_KEY_TEXT_RESET_MODE", NK_KEY_TEXT_RESET_MODE},
				{"NK_KEY_TEXT_LINE_START", NK_KEY_TEXT_LINE_START},
				{"NK_KEY_TEXT_LINE_END", NK_KEY_TEXT_LINE_END},
				{"NK_KEY_TEXT_START", NK_KEY_TEXT_START},
				{"NK_KEY_TEXT_END", NK_KEY_TEXT_END},
				{"NK_KEY_TEXT_UNDO", NK_KEY_TEXT_UNDO},
				{"NK_KEY_TEXT_REDO", NK_KEY_TEXT_REDO},
				{"NK_KEY_TEXT_SELECT_ALL", NK_KEY_TEXT_SELECT_ALL},
				{"NK_KEY_TEXT_WORD_LEFT", NK_KEY_TEXT_WORD_LEFT},
				{"NK_KEY_TEXT_WORD_RIGHT", NK_KEY_TEXT_WORD_RIGHT},
				{"NK_KEY_SCROLL_START", NK_KEY_SCROLL_START},
				{"NK_KEY_SCROLL_END", NK_KEY_SCROLL_END},
				{"NK_KEY_SCROLL_DOWN", NK_KEY_SCROLL_DOWN},
				{"NK_KEY_SCROLL_UP", NK_KEY_SCROLL_UP},
				{"NK_KEY_MAX", NK_KEY_MAX}
			});

			lua.new_enum<nk_buttons>("nk_buttons",
			{
				{"NK_BUTTON_LEFT", NK_BUTTON_LEFT},
				{"NK_BUTTON_MIDDLE", NK_BUTTON_MIDDLE},
				{"NK_BUTTON_RIGHT", NK_BUTTON_RIGHT},
				{"NK_BUTTON_DOUBLE", NK_BUTTON_DOUBLE},
				{"NK_BUTTON_MAX", NK_BUTTON_MAX}
			});

			lua.new_enum<nk_anti_aliasing>("nk_anti_aliasing",
			{
				{"NK_ANTI_ALIASING_OFF", NK_ANTI_ALIASING_OFF},
				{"NK_ANTI_ALIASING_ON", NK_ANTI_ALIASING_ON}
			});

			lua.new_enum<nk_convert_result>("nk_convert_result",
			{
				{"NK_CONVERT_SUCCESS", NK_CONVERT_SUCCESS},
				{"NK_CONVERT_INVALID_PARAM", NK_CONVERT_INVALID_PARAM},
				{"NK_CONVERT_COMMAND_BUFFER_FULL", NK_CONVERT_COMMAND_BUFFER_FULL},
				{"NK_CONVERT_VERTEX_BUFFER_FULL", NK_CONVERT_VERTEX_BUFFER_FULL},
				{"NK_CONVERT_ELEMENT_BUFFER_FULL", NK_CONVERT_ELEMENT_BUFFER_FULL}
			});

			lua.new_enum<nk_panel_flags>("nk_panel_flags",
			{
				{"NK_WINDOW_BORDER", NK_WINDOW_BORDER},
				{"NK_WINDOW_MOVABLE", NK_WINDOW_MOVABLE},
				{"NK_WINDOW_SCALABLE", NK_WINDOW_SCALABLE},
				{"NK_WINDOW_CLOSABLE", NK_WINDOW_CLOSABLE},
				{"NK_WINDOW_MINIMIZABLE", NK_WINDOW_MINIMIZABLE},
				{"NK_WINDOW_NO_SCROLLBAR", NK_WINDOW_NO_SCROLLBAR},
				{"NK_WINDOW_TITLE", NK_WINDOW_TITLE},
				{"NK_WINDOW_SCROLL_AUTO_HIDE", NK_WINDOW_SCROLL_AUTO_HIDE},
				{"NK_WINDOW_BACKGROUND", NK_WINDOW_BACKGROUND},
				{"NK_WINDOW_SCALE_LEFT", NK_WINDOW_SCALE_LEFT},
				{"NK_WINDOW_NO_INPUT", NK_WINDOW_NO_INPUT}
			});

			lua.new_enum<nk_widget_layout_states>("nk_widget_layout_states",
			{
				{"NK_WIDGET_INVALID", NK_WIDGET_INVALID},
				{"NK_WIDGET_VALID", NK_WIDGET_VALID},
				{"NK_WIDGET_ROM", NK_WIDGET_ROM}
			});

			lua.new_enum<nk_widget_states>("nk_widget_states",
			{
				{"NK_WIDGET_STATE_MODIFIED", NK_WIDGET_STATE_MODIFIED},
				{"NK_WIDGET_STATE_INACTIVE", NK_WIDGET_STATE_INACTIVE},
				{"NK_WIDGET_STATE_ENTERED", NK_WIDGET_STATE_ENTERED},
				{"NK_WIDGET_STATE_HOVER", NK_WIDGET_STATE_HOVER},
				{"NK_WIDGET_STATE_ACTIVED", NK_WIDGET_STATE_ACTIVED},
				{"NK_WIDGET_STATE_LEFT", NK_WIDGET_STATE_LEFT},
				{"NK_WIDGET_STATE_HOVERED", NK_WIDGET_STATE_HOVERED},
				{"NK_WIDGET_STATE_ACTIVE", NK_WIDGET_STATE_ACTIVE}
			});

			lua.new_enum<nk_text_align>("nk_text_align",
			{
				{"NK_TEXT_ALIGN_LEFT", NK_TEXT_ALIGN_LEFT},
				{"NK_TEXT_ALIGN_CENTERED", NK_TEXT_ALIGN_CENTERED},
				{"NK_TEXT_ALIGN_RIGHT", NK_TEXT_ALIGN_RIGHT},
				{"NK_TEXT_ALIGN_TOP", NK_TEXT_ALIGN_TOP},
				{"NK_TEXT_ALIGN_MIDDLE", NK_TEXT_ALIGN_MIDDLE},
				{"NK_TEXT_ALIGN_BOTTOM", NK_TEXT_ALIGN_BOTTOM}
			});

			lua.new_enum<nk_text_alignment>("nk_text_alignment",
			{
				{"NK_TEXT_LEFT", NK_TEXT_LEFT},
				{"NK_TEXT_CENTERED", NK_TEXT_CENTERED},
				{"NK_TEXT_RIGHT", NK_TEXT_RIGHT}
			});

			lua.new_enum<nk_edit_flags>("nk_edit_flags",
			{
				{"NK_EDIT_DEFAULT", NK_EDIT_DEFAULT},
				{"NK_EDIT_READ_ONLY", NK_EDIT_READ_ONLY},
				{"NK_EDIT_AUTO_SELECT", NK_EDIT_AUTO_SELECT},
				{"NK_EDIT_SIG_ENTER", NK_EDIT_SIG_ENTER},
				{"NK_EDIT_ALLOW_TAB", NK_EDIT_ALLOW_TAB},
				{"NK_EDIT_NO_CURSOR", NK_EDIT_NO_CURSOR},
				{"NK_EDIT_SELECTABLE", NK_EDIT_SELECTABLE},
				{"NK_EDIT_CLIPBOARD", NK_EDIT_CLIPBOARD},
				{"NK_EDIT_CTRL_ENTER_NEWLINE", NK_EDIT_CTRL_ENTER_NEWLINE},
				{"NK_EDIT_NO_HORIZONTAL_SCROLL", NK_EDIT_NO_HORIZONTAL_SCROLL},
				{"NK_EDIT_ALWAYS_INSERT_MODE", NK_EDIT_ALWAYS_INSERT_MODE},
				{"NK_EDIT_MULTILINE", NK_EDIT_MULTILINE},
				{"NK_EDIT_GOTO_END_ON_ACTIVATE", NK_EDIT_GOTO_END_ON_ACTIVATE}
			});

			lua.new_enum<nk_edit_types>("nk_edit_types",
			{
				{"NK_EDIT_SIMPLE", NK_EDIT_SIMPLE},
				{"NK_EDIT_FIELD", NK_EDIT_FIELD},
				{"NK_EDIT_BOX", NK_EDIT_BOX},
				{"NK_EDIT_EDITOR", NK_EDIT_EDITOR}
			});

			lua.new_enum<nk_edit_events>("nk_edit_events",
			{
				{"NK_EDIT_ACTIVE", NK_EDIT_ACTIVE},
				{"NK_EDIT_INACTIVE", NK_EDIT_INACTIVE},
				{"NK_EDIT_ACTIVATED", NK_EDIT_ACTIVATED},
				{"NK_EDIT_DEACTIVATED", NK_EDIT_DEACTIVATED},
				{"NK_EDIT_COMMITED", NK_EDIT_COMMITED}
			});

			lua.new_enum<nk_style_colors>("nk_style_colors",
			{
				{"NK_COLOR_TEXT", NK_COLOR_TEXT},
				{"NK_COLOR_WINDOW", NK_COLOR_WINDOW},
				{"NK_COLOR_HEADER", NK_COLOR_HEADER},
				{"NK_COLOR_BORDER", NK_COLOR_BORDER},
				{"NK_COLOR_BUTTON", NK_COLOR_BUTTON},
				{"NK_COLOR_BUTTON_HOVER", NK_COLOR_BUTTON_HOVER},
				{"NK_COLOR_BUTTON_ACTIVE", NK_COLOR_BUTTON_ACTIVE},
				{"NK_COLOR_TOGGLE", NK_COLOR_TOGGLE},
				{"NK_COLOR_TOGGLE_HOVER", NK_COLOR_TOGGLE_HOVER},
				{"NK_COLOR_TOGGLE_CURSOR", NK_COLOR_TOGGLE_CURSOR},
				{"NK_COLOR_SELECT", NK_COLOR_SELECT},
				{"NK_COLOR_SELECT_ACTIVE", NK_COLOR_SELECT_ACTIVE},
				{"NK_COLOR_SLIDER", NK_COLOR_SLIDER},
				{"NK_COLOR_SLIDER_CURSOR", NK_COLOR_SLIDER_CURSOR},
				{"NK_COLOR_SLIDER_CURSOR_HOVER", NK_COLOR_SLIDER_CURSOR_HOVER},
				{"NK_COLOR_SLIDER_CURSOR_ACTIVE", NK_COLOR_SLIDER_CURSOR_ACTIVE},
				{"NK_COLOR_PROPERTY", NK_COLOR_PROPERTY},
				{"NK_COLOR_EDIT", NK_COLOR_EDIT},
				{"NK_COLOR_EDIT_CURSOR", NK_COLOR_EDIT_CURSOR},
				{"NK_COLOR_COMBO", NK_COLOR_COMBO},
				{"NK_COLOR_CHART", NK_COLOR_CHART},
				{"NK_COLOR_CHART_COLOR", NK_COLOR_CHART_COLOR},
				{"NK_COLOR_CHART_COLOR_HIGHLIGHT", NK_COLOR_CHART_COLOR_HIGHLIGHT},
				{"NK_COLOR_SCROLLBAR", NK_COLOR_SCROLLBAR},
				{"NK_COLOR_SCROLLBAR_CURSOR", NK_COLOR_SCROLLBAR_CURSOR},
				{"NK_COLOR_SCROLLBAR_CURSOR_HOVER", NK_COLOR_SCROLLBAR_CURSOR_HOVER},
				{"NK_COLOR_SCROLLBAR_CURSOR_ACTIVE", NK_COLOR_SCROLLBAR_CURSOR_ACTIVE},
				{"NK_COLOR_TAB_HEADER", NK_COLOR_TAB_HEADER},
				{"NK_COLOR_COUNT", NK_COLOR_COUNT}
			});

			lua.new_enum<nk_style_cursor>("nk_style_cursor",
			{
				{"NK_CURSOR_ARROW", NK_CURSOR_ARROW},
				{"NK_CURSOR_TEXT", NK_CURSOR_TEXT},
				{"NK_CURSOR_MOVE", NK_CURSOR_MOVE},
				{"NK_CURSOR_RESIZE_VERTICAL", NK_CURSOR_RESIZE_VERTICAL},
				{"NK_CURSOR_RESIZE_HORIZONTAL", NK_CURSOR_RESIZE_HORIZONTAL},
				{"NK_CURSOR_RESIZE_TOP_LEFT_DOWN_RIGHT", NK_CURSOR_RESIZE_TOP_LEFT_DOWN_RIGHT},
				{"NK_CURSOR_RESIZE_TOP_RIGHT_DOWN_LEFT", NK_CURSOR_RESIZE_TOP_RIGHT_DOWN_LEFT},
				{"NK_CURSOR_COUNT", NK_CURSOR_COUNT}
			});

			lua.new_enum<nk_font_coord_type>("nk_font_coord_type",
			{
				{"NK_COORD_UV", NK_COORD_UV},
				{"NK_COORD_PIXEL", NK_COORD_PIXEL}
			});

			lua.new_enum<nk_font_atlas_format>("nk_font_atlas_format",
			{
				{"NK_FONT_ATLAS_ALPHA8", NK_FONT_ATLAS_ALPHA8},
				{"NK_FONT_ATLAS_RGBA32", NK_FONT_ATLAS_RGBA32}
			});

			lua.new_enum<nk_allocation_type>("nk_allocation_type",
			{
				{"NK_BUFFER_FIXED", NK_BUFFER_FIXED},
				{"NK_BUFFER_DYNAMIC", NK_BUFFER_DYNAMIC}
			});

			lua.new_enum<nk_buffer_allocation_type>("nk_buffer_allocation_type",
			{
				{"NK_BUFFER_FRONT", NK_BUFFER_FRONT},
				{"NK_BUFFER_BACK", NK_BUFFER_BACK},
				{"NK_BUFFER_MAX", NK_BUFFER_MAX}
			});

			lua.new_enum<nk_text_edit_type>("nk_text_edit_type",
			{
				{"NK_TEXT_EDIT_SINGLE_LINE", NK_TEXT_EDIT_SINGLE_LINE},
				{"NK_TEXT_EDIT_MULTI_LINE", NK_TEXT_EDIT_MULTI_LINE}
			});

			lua.new_enum<nk_text_edit_mode>("nk_text_edit_mode",
			{
				{"NK_TEXT_EDIT_MODE_VIEW", NK_TEXT_EDIT_MODE_VIEW},
				{"NK_TEXT_EDIT_MODE_INSERT", NK_TEXT_EDIT_MODE_INSERT},
				{"NK_TEXT_EDIT_MODE_REPLACE", NK_TEXT_EDIT_MODE_REPLACE}
			});

			lua.new_enum<nk_command_type>("nk_command_type",
			{
				{"NK_COMMAND_NOP,", NK_COMMAND_NOP},
				{"NK_COMMAND_SCISSOR,", NK_COMMAND_SCISSOR},
				{"NK_COMMAND_LINE,", NK_COMMAND_LINE},
				{"NK_COMMAND_CURVE,", NK_COMMAND_CURVE},
				{"NK_COMMAND_RECT,", NK_COMMAND_RECT},
				{"NK_COMMAND_RECT_FILLED,", NK_COMMAND_RECT_FILLED},
				{"NK_COMMAND_RECT_MULTI_COLOR,", NK_COMMAND_RECT_MULTI_COLOR},
				{"NK_COMMAND_CIRCLE,", NK_COMMAND_CIRCLE},
				{"NK_COMMAND_CIRCLE_FILLED,", NK_COMMAND_CIRCLE_FILLED},
				{"NK_COMMAND_ARC,", NK_COMMAND_ARC},
				{"NK_COMMAND_ARC_FILLED,", NK_COMMAND_ARC_FILLED},
				{"NK_COMMAND_TRIANGLE,", NK_COMMAND_TRIANGLE},
				{"NK_COMMAND_TRIANGLE_FILLED,", NK_COMMAND_TRIANGLE_FILLED},
				{"NK_COMMAND_POLYGON,", NK_COMMAND_POLYGON},
				{"NK_COMMAND_POLYGON_FILLED,", NK_COMMAND_POLYGON_FILLED},
				{"NK_COMMAND_POLYLINE,", NK_COMMAND_POLYLINE},
				{"NK_COMMAND_TEXT,", NK_COMMAND_TEXT},
				{"NK_COMMAND_IMAGE,", NK_COMMAND_IMAGE},
				{"NK_COMMAND_CUSTOM", NK_COMMAND_CUSTOM}
			});

			lua.new_enum<nk_command_clipping>("nk_command_clipping",
			{
				{"NK_CLIPPING_OFF", NK_CLIPPING_OFF},
				{"NK_CLIPPING_ON", NK_CLIPPING_ON}
			});

			lua.new_enum<nk_draw_list_stroke>("nk_draw_list_stroke",
			{
				{"NK_STROKE_OPEN", NK_STROKE_OPEN},
				{"NK_STROKE_CLOSED", NK_STROKE_CLOSED}
			});

			lua.new_enum<nk_draw_vertex_layout_attribute>("nk_draw_vertex_layout_attribute",
			{
				{"NK_VERTEX_POSITION", NK_VERTEX_POSITION},
				{"NK_VERTEX_COLOR", NK_VERTEX_COLOR},
				{"NK_VERTEX_TEXCOORD", NK_VERTEX_TEXCOORD},
				{"NK_VERTEX_ATTRIBUTE_COUNT", NK_VERTEX_ATTRIBUTE_COUNT}
			});

			lua.new_enum<nk_draw_vertex_layout_format>("nk_draw_vertex_layout_format",
			{
				{"NK_FORMAT_SCHAR", NK_FORMAT_SCHAR},
				{"NK_FORMAT_SSHORT", NK_FORMAT_SSHORT},
				{"NK_FORMAT_SINT", NK_FORMAT_SINT},
				{"NK_FORMAT_UCHAR", NK_FORMAT_UCHAR},
				{"NK_FORMAT_USHORT", NK_FORMAT_USHORT},
				{"NK_FORMAT_UINT", NK_FORMAT_UINT},
				{"NK_FORMAT_FLOAT", NK_FORMAT_FLOAT},
				{"NK_FORMAT_DOUBLE", NK_FORMAT_DOUBLE},
				{"NK_FORMAT_COLOR_BEGIN", NK_FORMAT_COLOR_BEGIN},
				{"NK_FORMAT_R8G8B8", NK_FORMAT_R8G8B8},
				{"NK_FORMAT_R16G15B16", NK_FORMAT_R16G15B16},
				{"NK_FORMAT_R32G32B32", NK_FORMAT_R32G32B32},
				{"NK_FORMAT_R8G8B8A8", NK_FORMAT_R8G8B8A8},
				{"NK_FORMAT_B8G8R8A8", NK_FORMAT_B8G8R8A8},
				{"NK_FORMAT_R16G15B16A16", NK_FORMAT_R16G15B16A16},
				{"NK_FORMAT_R32G32B32A32", NK_FORMAT_R32G32B32A32},
				{"NK_FORMAT_R32G32B32A32_FLOAT", NK_FORMAT_R32G32B32A32_FLOAT},
				{"NK_FORMAT_R32G32B32A32_DOUBLE", NK_FORMAT_R32G32B32A32_DOUBLE},
				{"NK_FORMAT_RGB32", NK_FORMAT_RGB32},
				{"NK_FORMAT_RGBA32", NK_FORMAT_RGBA32},
				{"NK_FORMAT_COLOR_END", NK_FORMAT_COLOR_END},
				{"NK_FORMAT_COUNT", NK_FORMAT_COUNT}
			});

			lua.new_enum<nk_style_item_type>("nk_style_item_type",
			{
				{"NK_STYLE_ITEM_COLOR", NK_STYLE_ITEM_COLOR},
				{"NK_STYLE_ITEM_IMAGE", NK_STYLE_ITEM_IMAGE},
				{"NK_STYLE_ITEM_NINE_SLICE", NK_STYLE_ITEM_NINE_SLICE}
			});

			lua.new_enum<nk_style_header_align>("nk_style_header_align",
			{
				{"NK_HEADER_LEFT", NK_HEADER_LEFT},
				{"NK_HEADER_RIGHT", NK_HEADER_RIGHT}
			});

			lua.new_enum<nk_panel_type>("nk_panel_type",
			{
				{"NK_PANEL_NONE", NK_PANEL_NONE},
				{"NK_PANEL_WINDOW", NK_PANEL_WINDOW},
				{"NK_PANEL_GROUP", NK_PANEL_GROUP},
				{"NK_PANEL_POPUP", NK_PANEL_POPUP},
				{"NK_PANEL_CONTEXTUAL", NK_PANEL_CONTEXTUAL},
				{"NK_PANEL_COMBO", NK_PANEL_COMBO},
				{"NK_PANEL_MENU", NK_PANEL_MENU},
				{"NK_PANEL_TOOLTIP", NK_PANEL_TOOLTIP}
			});

			lua.new_enum<nk_panel_set>("nk_panel_set",
			{
				{"NK_PANEL_SET_NONBLOCK", NK_PANEL_SET_NONBLOCK},
				{"NK_PANEL_SET_POPUP", NK_PANEL_SET_POPUP},
				{"NK_PANEL_SET_SUB", NK_PANEL_SET_SUB}
			});

			lua.new_enum<nk_panel_row_layout_type>("nk_panel_row_layout_type",
			{
				{"NK_LAYOUT_DYNAMIC_FIXED", NK_LAYOUT_DYNAMIC_FIXED},
				{"NK_LAYOUT_DYNAMIC_ROW", NK_LAYOUT_DYNAMIC_ROW},
				{"NK_LAYOUT_DYNAMIC_FREE", NK_LAYOUT_DYNAMIC_FREE},
				{"NK_LAYOUT_DYNAMIC", NK_LAYOUT_DYNAMIC},
				{"NK_LAYOUT_STATIC_FIXED", NK_LAYOUT_STATIC_FIXED},
				{"NK_LAYOUT_STATIC_ROW", NK_LAYOUT_STATIC_ROW},
				{"NK_LAYOUT_STATIC_FREE", NK_LAYOUT_STATIC_FREE},
				{"NK_LAYOUT_STATIC", NK_LAYOUT_STATIC},
				{"NK_LAYOUT_TEMPLATE", NK_LAYOUT_TEMPLATE},
				{"NK_LAYOUT_COUNT", NK_LAYOUT_COUNT}
			});

			lua.new_enum<nk_window_flags>("nk_window_flags",
			{
				{"NK_WINDOW_PRIVATE", NK_WINDOW_PRIVATE},
				{"NK_WINDOW_DYNAMIC", NK_WINDOW_DYNAMIC},
				{"NK_WINDOW_ROM", NK_WINDOW_ROM},
				{"NK_WINDOW_NOT_INTERACTIVE", NK_WINDOW_NOT_INTERACTIVE},
				{"NK_WINDOW_HIDDEN", NK_WINDOW_HIDDEN},
				{"NK_WINDOW_CLOSED", NK_WINDOW_CLOSED},
				{"NK_WINDOW_MINIMIZED", NK_WINDOW_MINIMIZED},
				{"NK_WINDOW_REMOVE_ROM", NK_WINDOW_REMOVE_ROM}
			});
			// clang-format on

			lua.new_usertype<nk_context>("nk_context", sol::no_constructor);
			lua.new_usertype<nk_style_item_data>("nk_style_item_data", sol::default_constructor);

			auto nkrect_type = lua.new_usertype<struct nk_rect>("nk_rect", sol::default_constructor);
			nkrect_type["x"] = &nk_rect::x;
			nkrect_type["y"] = &nk_rect::y;
			nkrect_type["w"] = &nk_rect::w;
			nkrect_type["h"] = &nk_rect::h;

			auto nknineslice_type   = lua.new_usertype<nk_nine_slice>("nk_nine_slice", sol::default_constructor);
			nknineslice_type["img"] = &nk_nine_slice::img;
			nknineslice_type["r"]   = &nk_nine_slice::r;
			nknineslice_type["b"]   = &nk_nine_slice::b;
			nknineslice_type["l"]   = &nk_nine_slice::l;
			nknineslice_type["t"]   = &nk_nine_slice::t;

			auto nkimage_type = lua.new_usertype<struct nk_image>("nk_image", sol::default_constructor);
			nkimage_type["w"] = &nk_image::w;
			nkimage_type["h"] = &nk_image::h;

			auto nkstyleitem_type    = lua.new_usertype<nk_style_item>("nk_style_item", sol::default_constructor);
			nkstyleitem_type["data"] = &nk_style_item::data;
			nkstyleitem_type["type"] = &nk_style_item::type;

			auto nkcolor_type = lua.new_usertype<nk_color>("nk_color", sol::default_constructor);
			nkcolor_type["r"] = &nk_color::r;
			nkcolor_type["g"] = &nk_color::g;
			nkcolor_type["b"] = &nk_color::b;
			nkcolor_type["a"] = &nk_color::a;

			lua.set_function("nk_rect", &nk_rect);
			lua.set_function("nk_begin", &nk_begin);
			lua.set_function("nk_end", &nk_end);
			lua.set_function("nk_layout_row_static", &nk_layout_row_static);
			lua.set_function("nk_layout_row_dynamic", &nk_layout_row_dynamic);
			lua.set_function("nk_layout_row_begin", &nk_layout_row_begin);
			lua.set_function("nk_layout_row_end", &nk_layout_row_end);
			lua.set_function("nk_image", &nk_image);
			lua.set_function("nk_style_item_color", &nk_style_item_color);
			lua.set_function("nk_rgb", &nk_rgb);
			lua.set_function("nk_style_item_hide", &nk_style_item_hide);
			lua.set_function("nk_style_item_image", &nk_style_item_image);
		}
	} // namespace lua
} // namespace galaxy
