#pragma once

#include "includes.h"

#include "element.h"

class c_button : public c_element
{

private:

    ImVec2 m_size{};

public:

    inline c_button(str_ref& name, str_ref& label, const ImVec2& size = ImVec2(0, 0), const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_size = size;
        this->m_visible = default_visibility;
        this->m_type = element_type::BUTTON;

    }

    virtual bool render() { return ImGui::Button(m_label.c_str(), m_size); }

};
