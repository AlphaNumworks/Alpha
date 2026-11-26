apps += Sprites::App
app_headers += apps/sprites/app.h

SFLAGS += -DHAS_SPRITE

app_sprites_src = $(addprefix apps/sprites/,\
	app.cpp \
	\
	sprite/sprite.cpp \
	\
	controllers/menu_controller.cpp \
	controllers/sprite_parameter_controller.cpp \
	controllers/cells/sprite_name_cell.cpp \
	\
	store/sprite_store.cpp \
)

# app_code_test_src = $(addprefix apps/code/,\
# 	python_toolbox.cpp \
# 	script.cpp \
# 	script_node_cell.cpp \
# 	script_store.cpp \
# 	script_template.cpp \
# 	variable_box_empty_controller.cpp \
# 	variable_box_controller.cpp \
# )

# tests_src += $(addprefix apps/code/test/,\
# 	variable_box_controller.cpp\
# 	toolbox_ion_keys_dummy.cpp \
# )

# app_code_src += $(app_code_test_src)
apps_src += $(app_sprites_src)

i18n_files += $(call i18n_with_universal_for,sprites/i18n/base)
# i18n_files += $(call i18n_with_universal_for,sprites/catalog)
# i18n_files += $(call i18n_with_universal_for,sprites/toolbox)

$(eval $(call depends_on_image,apps/sprites/app.cpp,apps/sprites/sprites_icon.png))
