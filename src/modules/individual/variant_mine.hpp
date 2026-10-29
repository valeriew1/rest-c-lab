#pragma once
// ============================================================================
// ВАРИАНТ 23: Labor Productivity Indexer (Эффективность использования времени)
// ============================================================================

#include <string>
#include <cmath>
#include "../../json.hpp"

namespace TaskVariant {

    struct Input {
        double total_hours_paid;
        double logged_project_hours;
        double total_revenue_generated;
        double overtime_hours_paid;
    };

    struct Output {
        double utilization_rate;
        double revenue_per_productive_hour;
        double overtime_cost_impact;
    };

    inline std::string validate(const Input& in) {
        if (in.total_hours_paid <= 0.0)
            return "total_hours_paid must be > 0.0";
        if (in.logged_project_hours < 0.0 || in.logged_project_hours > in.total_hours_paid)
            return "logged_project_hours must be in [0.0, total_hours_paid]";
        if (in.total_revenue_generated < 0.0)
            return "total_revenue_generated must be >= 0.0";
        if (in.overtime_hours_paid < 0.0 || in.overtime_hours_paid > in.total_hours_paid * 0.5)
            return "overtime_hours_paid must be in [0.0, total_hours_paid * 0.5]";
        return "";
    }

    inline Output calculate(const Input& in) {
        Output out{};
        out.utilization_rate = in.logged_project_hours / in.total_hours_paid;
        
        if (in.logged_project_hours > 0.0) {
            out.revenue_per_productive_hour = in.total_revenue_generated / in.logged_project_hours;
        } else {
            out.revenue_per_productive_hour = 0.0;
        }
        
        out.overtime_cost_impact = (in.overtime_hours_paid * 1.5) / in.total_hours_paid;
        return out;
    }

    inline Input parse(const std::string& body) {
        Input in{};
        auto get_d = [&](const std::string& key) -> double {
            auto p = body.find("\"" + key + "\"");
            if (p == std::string::npos) return 0.0;
            p = body.find(':', p);
            if (p == std::string::npos) return 0.0;
            try { return std::stod(body.substr(p + 1)); }
            catch (...) { return 0.0; }
        };
        in.total_hours_paid        = get_d("total_hours_paid");
        in.logged_project_hours    = get_d("logged_project_hours");
        in.total_revenue_generated = get_d("total_revenue_generated");
        in.overtime_hours_paid     = get_d("overtime_hours_paid");
        return in;
    }

    inline std::string to_json(const Output& out) {
        JObj o;
        o.num("utilization_rate", out.utilization_rate);
        o.num("revenue_per_productive_hour", out.revenue_per_productive_hour);
        o.num("overtime_cost_impact", out.overtime_cost_impact);
        return o.str();
    }

} // namespace TaskVariant