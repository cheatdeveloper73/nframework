#pragma once

#include "includes.h"

enum element_type : uint16_t
{

    LABEL,
    CHECKBOX,
    SLIDER_INT,
    SLIDER_FLOAT,
    SINGLE_COMBO,
    MULTI_COMBO,
    BUTTON,
    DIVIDER,
    LABEL_DIVIDER,
    SINGLE_LISTBOX,
    MULTI_LISTBOX,
    COLORPICKER,
    KEYBIND,
    TAB,
    GROUP,
    SWITCH,

};

class c_element
{

    // look at all this guy's friends... woah!

    friend class c_window;
    friend class c_child;

    friend class c_label;
    friend class c_checkbox;
    friend class c_slider_int;
    friend class c_slider_float;
    friend class c_combo;
    friend class c_multi_combo;
    friend class c_button;
    friend class c_divider;
    friend class c_label_divider;
    friend class c_single_listbox;
    friend class c_multi_listbox;
    friend class c_colorpicker;
    friend class c_keybind;
    friend class c_tab;
    friend class c_group;
    friend class c_toggle;
    friend class c_collapsable_group;

private:

    std::string m_name{};
    std::string m_label{};
    std::function<bool(void)> m_visibility_callback{};
    std::function<void(void)> m_render_callback{};
    std::function<void(void)> m_interaction_callback{};
    bool m_visible{};
    bool m_wants_same_line{};
    element_type m_type{};

    void update_visibility(const bool force_visibility = false) noexcept
    {

        if (!force_visibility && m_visibility_callback) // automatically set visibility based on the callback, else force visibility via parameter.
            m_visible = m_visibility_callback();
        else if (!force_visibility && m_visibility_callback)
            m_visible = force_visibility;

    }

    void run_render_callback() const noexcept
    {

        if (m_render_callback)
            m_render_callback();

    }

    void run_interaction_callback() const noexcept
    {

        if (m_interaction_callback)
            m_interaction_callback();

    }

    virtual bool render() = 0;

public:

    // dis hoe aint need no constructor...

    bool get_visible() const noexcept { return m_visible; }
    void set_visible(const bool value) noexcept { m_visible = value; }

    void set_visibility_callback(const std::function<bool(void)>& value) noexcept { m_visibility_callback = value; } // callback to allow users to change visibility based on variables
    void set_render_callback(const std::function<void(void)>& value) noexcept { m_render_callback = value; } // callback before rendering of the element
    void set_interaction_callback(const std::function<void(void)>& value) noexcept { m_interaction_callback = value; } // callback on interaction with an element

    std::string get_name() const noexcept { return m_name; }

    void set_label(str_ref& label) noexcept { m_label = label; }

    void set_same_line(bool same_line) noexcept { m_wants_same_line = same_line; }

};
