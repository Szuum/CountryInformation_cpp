#include <iostream>
#include "single_include/nlohmann/json.hpp"
using json = nlohmann::json;
#include <cpr/cpr.h>
#include <curl/curl.h>
#include <time.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_JPEG_Image.H>

using namespace std;

Fl_Window* main_window;
Fl_Input* input_field;
Fl_Button* button;
Fl_Box* official_name;
Fl_Box* currencies;
Fl_Box* capital_name;
Fl_Box* region;
Fl_Box* area;
Fl_Box* flag_box;
Fl_Box* population;
Fl_Box* current_time;
Fl_JPEG_Image* flag;
json countries;
string official_name_label;
string currencies_label;
string capital_name_label;
string region_label;
string area_label;
string population_label;
string current_time_label;
time_t my_time;
struct tm *local_time;

string remove_quotes(string str) {
    return str.substr(1, str.size() - 2);
}

void download_flag(const char* filename, const char* url) {
    CURL* flag_download;
    flag_download = curl_easy_init();
    FILE* file = fopen(filename, "wb");
    curl_easy_setopt(flag_download, CURLOPT_URL, url);
    curl_easy_setopt(flag_download, CURLOPT_WRITEDATA, file);
    curl_easy_perform(flag_download);
    curl_easy_cleanup(flag_download);
    fclose(file);
}

string get_inf(int i, int type_num) {
    string type;
    string inf;
    switch (type_num) {
        case 1:
            type = "Official name: ";
            inf = remove_quotes(to_string(countries[i]["name"]["official"]));
            break;
        case 2:
            type = "Currencies: ";
            inf = countries[i]["currencies"].begin().key();
            break;
        case 3:
            type = "Capital name: ";
            inf = remove_quotes(to_string(countries[i]["capital"][0]));
            break;
        case 4:
            type = "Region: ";
            inf = remove_quotes(to_string(countries[i]["subregion"]));
            break;
        case 5:
            type = "Area [km^2]: ";
            inf = to_string(countries[i]["area"]);
            break;
        case 6:
            type = "Population: ";
            inf = to_string(countries[i]["population"]);
            break;
        default:
            if (to_string(countries[i]["timezones"][1]) != "null") {
                type = "There are more than one timezone";
                inf = "";
            }
            else {
                time(&my_time);
                local_time = localtime(&my_time);
                string timezone = remove_quotes(to_string(countries[i]["timezones"][0]));
                char sign = timezone[3];
                int utc_hour, utc_min;
                if (sign == '\0') {
                    utc_hour = 0;
                    utc_min = 0;
                    sign = '-';
                } else {
                    utc_hour = stoi(timezone.substr(4, 5));
                    utc_min = stoi(timezone.substr(7, 8));
                }
                int hour = local_time->tm_hour;
                int min = local_time->tm_min;
                int country_hour, country_min;
                if (sign == '-') {
                    country_min = min - utc_min;
                    country_hour = hour - utc_hour - 1;
                    if (country_min < 0) {
                        country_min += 60;
                        country_hour--;
                    }
                    if (country_hour < 0) {
                        country_hour += 24;
                    }
                } else {
                    country_min = min + utc_min;
                    country_hour = hour + utc_hour - 1;
                    if (country_min >= 60) {
                        country_min -= 60;
                        country_hour++;
                    }
                    if (country_hour >= 24) {
                        country_hour -= 24;
                    }
                }
                type = "Current time: ";
                if (country_min < 10) {
                    inf = to_string(country_hour) + ":0" + to_string(country_min);
                } else {
                    inf = to_string(country_hour) + ":" + to_string(country_min);
                }
            }
    }
    return type + inf;
}

void show_information(Fl_Widget* widget, void* data) {
    const char* selected_country = input_field->value();
    string country_name;
    string type;
    string inf;
    string tmp;
    bool wrong_input = true;
    for (int i = 0 ; i < 250 ; i++) {
        country_name = countries[i]["name"]["common"];
        if (country_name == selected_country) {
            wrong_input = false;

            official_name_label = get_inf(i, 1);
            official_name->label(official_name_label.c_str());

            currencies_label = get_inf(i, 2);
            currencies->label(currencies_label.c_str());

            capital_name_label = get_inf(i, 3);
            capital_name->label(capital_name_label.c_str());

            region_label = get_inf(i, 4);
            region->label(region_label.c_str());

            area_label = get_inf(i, 5);
            area->label(area_label.c_str());

            population_label = get_inf(i, 6);
            population->label(population_label.c_str());

            current_time_label = get_inf(i, 7);
            current_time->label(current_time_label.c_str());

            string url = remove_quotes(to_string(countries[i]["flags"]["png"]));
            url[url.length() - 3] = 'j';
            url[url.length() - 2] = 'p';
            url[url.length() - 1] = 'g';
            download_flag("flag.jpg", url.c_str());
            Fl_JPEG_Image* new_flag = new Fl_JPEG_Image("flag.jpg");
            delete flag;
            flag = new_flag;
            flag_box->image(flag);
            flag_box->parent()->redraw();
            break;
        }
    }
    if (wrong_input) {
        Fl_Window* new_window = new Fl_Window(400, 100, "Wrong input");
        Fl_Box* error_box = new Fl_Box(0, 10, 400, 40, "Wrong argument. Input common country name.");
        Fl_Box* error_box2 = new Fl_Box(0, 50, 400, 40, "Remember to start with a capital letter.");
        new_window->show();
    }
}

void init_window() {
    main_window = new Fl_Window(800, 300);
    input_field = new Fl_Input(150, 10, 200, 30, "Enter country name");
    button = new Fl_Button(10, 50, 200, 50, "Show information");
    button->callback(show_information);

    official_name = new Fl_Box(0, 120, 400, 20);

    currencies= new Fl_Box(0, 140, 400, 20);

    capital_name = new Fl_Box(0, 160, 400, 20);

    region = new Fl_Box(0, 180, 400, 20);

    area = new Fl_Box(0, 200, 400, 20);

    population = new Fl_Box(0, 220, 400, 20);

    current_time = new Fl_Box(0, 240, 400, 20);

    flag_box = new Fl_Box(0, 0, 500, 200);
    flag_box->position(300, 50);
}

int main() {
    cpr::Response r = cpr::Get(cpr::Url("https://restcountries.com/v3.1/all"));
    if (r.status_code != 200) {
        cout << "Cannot get data from https://restcountries.com/v3.1/all" << endl;
        return -1;
    }
    countries = json::parse(r.text);

    init_window();
    main_window->show();
    
    return Fl::run();
}