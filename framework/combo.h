#pragma once

#include "element.h"

class c_combo : public c_element
{

private:

    std::vector<std::string> m_combo_elements{};
    int* m_value{};

public:

    inline c_combo(const str_ref& name, const str_ref& label, int* value, const std::vector<std::string> combo_elements, const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_value = value;
        this->m_combo_elements = combo_elements;
        this->m_type = element_type::SINGLE_COMBO;

    }

    int get_value() const noexcept { return *m_value; }
    void set_value(const int value) noexcept { *m_value = value; }

    virtual bool render()
    {

        if (*m_value > m_combo_elements.size())
            return false;

        ImGui::Text(m_label.c_str());

        if (const auto selected = ImGui::BeginCombo(std::string("##vis_" + m_label).c_str(), m_combo_elements.at(*m_value).c_str(), NULL))
        {

            for (int i = 0; i < m_combo_elements.size(); i++)
            {

                bool is_selected = (*m_value == i);

                if (ImGui::Selectable(m_combo_elements.at(i).c_str(), &is_selected))
                    *m_value = i;

            }

            ImGui::EndCombo();

            return selected;

        }

        return false;

    }

};

class c_multi_combo : public c_element
{

private:

    std::vector<std::string> m_combo_elements{};
    std::vector<BYTE>* m_values{};

public:

    inline c_multi_combo(str_ref& name, str_ref& label, std::vector<BYTE>* values, const std::vector<std::string> combo_elements, const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_values = values;
        this->m_combo_elements = combo_elements;
        this->m_type = element_type::MULTI_COMBO;

    }

    std::vector<BYTE> get_values() const { return *m_values; }
    void set_value(const int index, const BYTE value) { m_values->at(index) = value; }

    int get_active_values() const noexcept
    {

        int active_values = 0;

        for (const auto value : *m_values)
            if (value) ++active_values; // true = active, false = inactive (duh)

        return active_values;

    }

    virtual bool render()
    {

        if (m_values->size() != m_combo_elements.size())
            m_values->resize(m_combo_elements.size());

        ImGui::Text(m_label.c_str());

        if (const auto selected = ImGui::BeginCombo(std::string("##vis_" + m_label).c_str(), (std::to_string(get_active_values()) + " Selected").c_str(), NULL))
        {

            for (int i = 0; i < m_combo_elements.size(); i++)
                ImGui::Selectable(m_combo_elements.at(i).c_str(), (bool*)&m_values->at(i), ImGuiSelectableFlags_DontClosePopups); // apply flag so it won't close when you select something

            ImGui::EndCombo();

        }

        return false;

    }

};
