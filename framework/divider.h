#pragma once

#include "element.h"

class c_divider : public c_element
{
private:
public:

    inline c_divider(str_ref& name, str_ref& label, const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_type = element_type::DIVIDER;

    }

    virtual bool render() { ImGui::Separator(); return false; }

};

class c_label_divider : public c_element
{
private:
public:

    inline c_label_divider(str_ref& name, str_ref& label, const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_type = element_type::LABEL_DIVIDER;

    }

    virtual bool render() { ImGui::SeparatorText(m_label.c_str()); return false; }

};
