local test = {}

function test:do_ui(self)
	if nk_begin(self.ctx, "Test", nk_rect(50, 50, 220, 220), 0) then
		nk_layout_row_static(self.ctx, 30, 80, 1)
		if nk_button_label(self.ctx, "button") then
			print("test")
		end
	end
	nk_end(self.ctx)
end

return test