#pragma once

#include "element.h"

class c_multi_listbox : public c_element
{

private:

    ImVec2 m_size{};
    std::vector<std::string> m_items{};
    std::vector<BYTE>* m_values{};

public:

    inline c_multi_listbox(str_ref& name, str_ref& label, std::vector<BYTE>* values, const std::vector<std::string>& items, const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_items = items;
        this->m_values = values;
        this->m_type = element_type::MULTI_LISTBOX;

    }

    virtual bool render()
    {

        ImGui::Text(m_label.c_str());

        m_size.x = ImGui::GetContentRegionAvail().x;

        if (m_values->size() != m_items.size())
            m_values->resize(m_items.size());

        if (const bool selected = ImGui::BeginListBox(std::string("##vis_" + m_label).c_str(), m_size))
        {

            for (int i = 0; i < m_items.size(); i++)
                ImGui::Selectable(m_items[i].c_str(), (bool*)&m_values->at(i), ImGuiSelectableFlags_SpanAvailWidth);

            ImGui::EndListBox();

            return selected;

        }

        return false;

    }

};

class c_single_listbox : public c_element
{
private:

    ImVec2 m_size{};
    std::vector<std::string> m_items{};
    int* m_selected{};

public:

    inline c_single_listbox(str_ref& name, str_ref& label, int* value, const std::vector<std::string>& items, const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_items = items;
        this->m_selected = value;
        this->m_type = element_type::SINGLE_LISTBOX;

    }

    virtual bool render()
    {

        ImGui::Text(m_label.c_str());

        m_size.x = ImGui::GetContentRegionAvail().x;

        if (const bool selected = ImGui::BeginListBox(std::string("##vis_" + m_label).c_str(), m_size))
        {

            for (int i = 0; i < m_items.size(); i++)
            {

                auto selected = (i == *m_selected);

                if (ImGui::Selectable(m_items[i].c_str(), &selected, ImGuiSelectableFlags_SpanAvailWidth))
                    *m_selected = i;

            }

            ImGui::EndListBox();

            return selected;

        }

        return false;

    }

    int get_value() const noexcept { return *m_selected; }

};
