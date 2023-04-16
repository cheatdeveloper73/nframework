#pragma once

#include "element.h"

class c_slider_int : public c_element
{

private:

    std::string m_format_text{ "%d" };
    int* m_value{};
    int m_min{}, m_max{};

public:

    inline c_slider_int(str_ref& name, str_ref& label, int* value, int min, int max, str_ref& format_str = "%.2f", const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_value = value;
        this->m_min = min;
        this->m_max = max;
        this->m_type = element_type::SLIDER_INT;
        this->m_format_text = format_str;

    }

    int get_min() const noexcept { return m_min; }
    void set_min(const int value) noexcept { m_min = value; }

    int get_max() const noexcept { return m_max; }
    void set_max(const int value) noexcept { m_max = value; }

    int get_value() const noexcept { return *m_value; }
    void set_value(const int value) noexcept { *m_value = value; }

    void set_format_text(str_ref& style) noexcept { m_format_text = style; }

    virtual bool render()
    {

        ImGui::Text(m_label.c_str());

        const auto ret = ImGui::SliderInt(std::string("##vis_" + m_label).c_str(), m_value, m_min, m_max, m_format_text.c_str());

        return ret;

    }

};

class c_slider_float : public c_element
{

private:

    std::string m_format_text{};
    float* m_value{};
    float m_min{}, m_max{};
    
public:

    inline c_slider_float(str_ref& name, str_ref& label, float* value, float min, float max, str_ref& format_str = "%.2f", const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_value = value;
        this->m_min = min;
        this->m_max = max;
        this->m_type = element_type::SLIDER_FLOAT;
        this->m_format_text = format_str;

    }

    float get_min() const noexcept { return m_min; }
    void set_min(const float value) noexcept { m_min = value; }

    float get_max() const noexcept { return m_max; }
    void set_max(const float value) noexcept { m_max = value; }

    float get_value() const noexcept { return *m_value; }
    void set_value(const float value) noexcept { *m_value = value; }

    void set_format_text(str_ref& text) noexcept { m_format_text = text; }

    virtual bool render()
    {

        ImGui::Text(m_label.c_str());

        const auto ret = ImGui::SliderFloat(std::string("##vis_" + m_label).c_str(), m_value, m_min, m_max, m_format_text.c_str());

        return ret;

    }

};
