#pragma once
#include "icg_helper.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
using namespace glm;

class Bezier {

    private:
        struct ppoint {
          vec3 curr;
          float from_start = 0;
          float from_prev;
          ppoint(vec3 one, float two, float three){
              curr = one;
              from_start = three;
              from_prev = two;
          }
        } ;
        vector<vec3> points;
        vector<ppoint> path_points;
        int last_used_pp = 1;
        float precision = 1000;
        float t = 0;
        float length = 0;
        float steps = 400;

        int binom(int n, int k){
           return  fact(n)/(fact(n-k)*fact(k));
        }
        int fact(int n){
           return  n == 0? 1 : n*fact(n-1);
        }
        vec3 curve(int n, float t){
            vec3 result = vec3(0,0,0.0f);
            for(int i = 0; i <points.size(); i++)
                result += points[i]*float(binom(points.size()-1,i)*pow((1-t), points.size()-1 - i)*pow(t,i));
            return result;
        }

        void measure_length(){
            length = path_points[path_points.size() - 2].from_start;
        }
        void make_path(){
            path_points.clear();
            float total = 0;
            vec3 prev = points[0];
            vec3 here;
            for(int i = 1; i<=precision; i++){
                here = curve(points.size(),float(i)/(precision));
                path_points.push_back(ppoint(here,glm::distance(here,prev),total+=glm::distance(here,prev)));
                prev = here;
            }
            //path_points.push_back(ppoint(points[points.size() - 1],glm::distance(points[points.size() - 1],prev),total +=glm::distance(points[points.size() - 1],prev) + 1));
        }

    public:

        float Leng(){return length;}
        vec3 Pos(){
            float curr_pos = t*length;
           // if(last_used_pp > precision-0.01*precision) return mix(path_points[last_used_pp-1].curr, path_points[last_used_pp].curr, (curr_pos-path_points[last_used_pp-1].from_start)/path_points[last_used_pp].from_prev);;

            while(path_points[last_used_pp].from_start < curr_pos)
                if((path_points.size() - 1) == last_used_pp )
                    break;
                else last_used_pp++ ;
            return mix(path_points[last_used_pp-1].curr, path_points[last_used_pp].curr, (curr_pos-path_points[last_used_pp-1].from_start)/path_points[last_used_pp].from_prev);
        }
        void Add_point(vec3 p){ points.push_back(p);}
        void Remove_point(){points.pop_back();}
        void Reset(){points.clear();}
        void SpeedUp(){steps /= 1.005;}
        void SlowDown(){steps *= 1.005;}
        int Make_step(){
            if(t < 1){
                t += 1/steps;
                return 1;
            }
            t = 0;
            length = 0;
            last_used_pp = 1;
            return 0;
        }
        void Start(){
            t = 0;
            steps = 400;
            make_path();
            measure_length();
            steps *= length;
            cout << "Length equals " << length << endl;
        }
        void Print(){
            cout << "------- Bezier -------" << endl;
            for(int i = 0; i < points.size(); i++)
                cout << i << ". (" << points[i].x << ", " << points[i].y << ", " << points[i].z << ")" << endl;
            cout << "----------------------" << endl;
        }
};

