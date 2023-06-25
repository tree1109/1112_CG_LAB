// ReSharper disable CppClangTidyClangDiagnosticInvalidUtf8
#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "GL/freeglut.h"
#include "Log.h"

class Object
{
private:
    M3DVector3d position_;
    M3DVector3d rotation_; // in radian
    double scale_;
    GLuint textureIndex_;
    std::string objPath_;
    std::string texturePath_;
    bool cull_face_;
    // model data
    std::vector<std::array<double, 3>> v_;
    std::vector<std::array<double, 3>> vt_;
    std::vector<std::array<double, 3>> vn_;
    std::vector<std::array<int, 4>> f_v_;
    std::vector<std::array<int, 4>> f_vt_;
    std::vector<std::array<int, 4>> f_vn_;
    bool is_paper_;

public:
    Object(const std::string& model_name) :
        position_{ 0.0, 0.0, 0.0 }, rotation_{ 0.0, 0.0, 0.0 }, scale_{ 1.0 },
        textureIndex_(0),
        objPath_("Model\\"+model_name+".obj"),
        texturePath_("Model\\" + model_name + ".png"),
        cull_face_(true),
        is_paper_(false)
    {}

    void LoadObject()
    {
        std::ifstream objFile(objPath_);
        if (!objFile.is_open()) {
            Log::Error("Could not open file: " + objPath_);
            return;
        }

        // create local vectors to store the data then
        // add a dummy vertex to the vertices vector
        std::array<double, 3> dummy = { 0, 0, 0 };
        v_.push_back(dummy);
        vt_.push_back(dummy);
        vn_.push_back(dummy);

        std::string lineBuffer;
        while (std::getline(objFile, lineBuffer)) {
            std::istringstream lineSS(lineBuffer);
            std::string token;

            // read the first token from the line
            lineSS >> token;
            if (token == "v") {
                // read the vertex data
                std::array<double, 3> vertex = {};
                lineSS >> vertex[0] >> vertex[1] >> vertex[2];
                v_.push_back(vertex);
            }
            else if (token == "vt") {
                // read the vertex data
                std::array<double, 3> vertex_texture = {};
                lineSS >> vertex_texture[0] >> vertex_texture[1] >> vertex_texture[2];
                vt_.push_back(vertex_texture);
            }
            else if (token == "vn") {
                // read the vertex data
                std::array<double, 3> vertex_normal = {};
                lineSS >> vertex_normal[0] >> vertex_normal[1] >> vertex_normal[2];
                vn_.push_back(vertex_normal);
            }
            else if (token == "f") {
                std::string temp;

                // read the face data
                std::array<int, 4> face_v = {};
                std::array<int, 4> face_vt = {};
                std::array<int, 4> face_vn = {};
                // NOTE: 這是為了將面的資料讀入，因為面的資料格式為 -> f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 v4/vt4/vn4
                {
                    lineSS >> temp;
                    std::replace(temp.begin(), temp.end(), '/', ' ');
                    std::stringstream SS(temp);
                    SS >> face_v[0] >> face_vt[0] >> face_vn[0];
                }
                {
                    lineSS >> temp;
                    std::replace(temp.begin(), temp.end(), '/', ' ');
                    std::stringstream SS(temp);
                    SS >> face_v[1] >> face_vt[1] >> face_vn[1];
                }
                {
                    lineSS >> temp;
                    std::replace(temp.begin(), temp.end(), '/', ' ');
                    std::stringstream SS(temp);
                    SS >> face_v[2] >> face_vt[2] >> face_vn[2];
                }
                {
                    lineSS >> temp;
                    std::replace(temp.begin(), temp.end(), '/', ' ');
                    std::stringstream SS(temp);
                    SS >> face_v[3] >> face_vt[3] >> face_vn[3];
                }
                f_v_.push_back(face_v);
                f_vt_.push_back(face_vt);
                f_vn_.push_back(face_vn);
            }
        }
        objFile.close();
    }

    void LoadTexture()
    {
        // 圖片使用帶有透明度的PNG影像
        cv::Mat image = cv::imread(texturePath_);
        if (image.empty())
        {
            Log::Error("Fail to load texture \"" + texturePath_ + "\".");
            return;
        }
        // 上下翻轉
        cv::flip(image, image, 0);
        // 由各個物件保管紋理索引
        glGenTextures(1, &textureIndex_);
        glBindTexture(GL_TEXTURE_2D, textureIndex_);
        // 設定紋理參數
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        // 設定紋理影像
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.ptr());
    }

    void DeleteTexture() const
    {
        glDeleteTextures(1, &textureIndex_);
    }

    void SetPaper(bool is_paper)
    {
        is_paper_ = is_paper;
    }

    void SetCullFace(bool cull_face)
    {
        cull_face_ = cull_face;
    }

    void SetScale(double scale)
    {
        scale_ = scale;
    }

    void SetPosition(const M3DVector3d& position)
    {
        m3dCopyVector3(position_, position);
    }

    void SetRotationDeg(const M3DVector3d& rotation)
    {
        m3dCopyVector3(rotation_, rotation);
    }

    void Draw(bool shadow = false) const
    {
        // 物件座標轉換
        glPushMatrix();
        glScaled(scale_, scale_, scale_);
        glRotated(rotation_[0], 1.0, 0.0, 0.0);
        glRotated(rotation_[1], 0.0, 1.0, 0.0);
        glRotated(rotation_[2], 0.0, 0.0, 1.0);
        // 消除scale造成的座標放大問題，使得平移單位與世界單位相同
        glTranslated(position_[0] / scale_, position_[1] / scale_, position_[2] / scale_); 
        
        // 開啟透明貼圖功能 
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // 繪製物件陰影
        if (shadow)
            glColor3d(0, 0, 0);
        else
            glColor3d(1, 1, 1);

        // 設定是否雙面顯示
        if (cull_face_)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);

        // 繪製物件
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureIndex_);
        glBegin(GL_QUADS);

        if (is_paper_)
        {
            glTexCoord2d(0, 0);
            glVertex3d(1, -1, 0);
            glTexCoord2d(0, 1);
            glVertex3d(1, 1, 0);
            glTexCoord2d(1, 1);
            glVertex3d(-1, 1, 0);
            glTexCoord2d(1, 0);
            glVertex3d(-1, -1, 0);
        }
        else
        {
            for (int i = 0; i < f_v_.size(); ++i)
            {
                auto& v1 = v_[f_v_[i][0]];
                auto& v2 = v_[f_v_[i][1]];
                auto& v3 = v_[f_v_[i][2]];
                auto& v4 = v_[f_v_[i][3]];
                auto& vt1 = vt_[f_vt_[i][0]];
                auto& vt2 = vt_[f_vt_[i][1]];
                auto& vt3 = vt_[f_vt_[i][2]];
                auto& vt4 = vt_[f_vt_[i][3]];
                auto& vn1 = vn_[f_vn_[i][0]];
                auto& vn2 = vn_[f_vn_[i][1]];
                auto& vn3 = vn_[f_vn_[i][2]];
                auto& vn4 = vn_[f_vn_[i][3]];
                glTexCoord2d(vt1[0], vt1[1]);
                glNormal3dv(vn1.data());
                glVertex3dv(v1.data());
                glTexCoord2d(vt2[0], vt2[1]);
                glNormal3dv(vn2.data());
                glVertex3dv(v2.data());
                glTexCoord2d(vt3[0], vt3[1]);
                glNormal3dv(vn3.data());
                glVertex3dv(v3.data());
                glTexCoord2d(vt4[0], vt4[1]);
                glNormal3dv(vn4.data());
                glVertex3dv(v4.data());
            }
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
        
        glEnable(GL_CULL_FACE);

        // 關閉透明貼圖功能
        glDisable(GL_BLEND);

        // 提供給子物件用的轉換，不包含scale
        glPopMatrix();
        glRotated(rotation_[0], 1.0, 0.0, 0.0);
        glRotated(rotation_[1], 0.0, 1.0, 0.0);
        glRotated(rotation_[2], 0.0, 0.0, 1.0);
        glTranslated(position_[0], position_[1], position_[2]);
    }
};
