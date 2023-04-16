#pragma once

#include "element.h"

class c_checkbox : public c_element
{

private:

    bool* m_value{};

public:

    inline c_checkbox(str_ref& name, str_ref& label, bool* value, const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_value = value;
        this->m_type = element_type::CHECKBOX;

    }

    bool get_value() const noexcept { return *m_value; }
    void set_value(bool value) noexcept { *m_value = value; }

    virtual bool render() { return ImGui::Checkbox(m_label.c_str(), m_value); }

};
