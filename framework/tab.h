#pragma once

#include "element.h"

// very similar to a button, except it toggles l0l
class c_tab : public c_element
{

private:

    ImVec2 m_size{};
    int* m_current_tab{};
    int m_this_tab{};

public:

    inline c_tab(str_ref& name, str_ref& label, const ImVec2& size, const int this_tab, int* current_tab, const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_type = element_type::TAB;
        this->m_current_tab = current_tab;
        this->m_this_tab = this_tab;
        this->m_size = size;

    }

    virtual bool render()
    {

        if (*m_current_tab == m_this_tab)
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));

        const auto ret = ImGui::Button(m_label.c_str(), m_size);

        if (*m_current_tab == m_this_tab)
            ImGui::PopStyleColor();

        if (ret) *m_current_tab = m_this_tab;

        return ret;

    }

};
