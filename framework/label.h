#pragma once

#include "element.h"

class c_label : public c_element
{

public:

    inline c_label(str_ref& name, str_ref& label, const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_type = element_type::LABEL;

    }

    virtual bool render() { ImGui::Text(m_label.c_str()); return false; /* return false cause how a label gon return true... */ }

};
