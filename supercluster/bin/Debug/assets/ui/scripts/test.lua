local test = {}

function test:do_ui()
	local flags = nk_panel_flags.NK_WINDOW_BORDER | nk_panel_flags.NK_WINDOW_MOVABLE | nk_panel_flags.NK_WINDOW_SCALABLE | nk_panel_flags.NK_WINDOW_MINIMIZABLE | nk_panel_flags.NK_WINDOW_TITLE

	if nk_begin(self.ctx, "Test", nk_rect(50, 50, 220, 220), flags) then
	end
	nk_end(self.ctx)
end

return test