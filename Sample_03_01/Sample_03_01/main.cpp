﻿#include "stdafx.h"
#include "system/system.h"
#include "TrianglePolygon.h"
#define _USE_MATH_DEFINES
#include <math.h>

//関数宣言
void InitRootSignature(RootSignature& rs);

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    //ゲームの初期化
    InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

    //////////////////////////////////////
    // ここから初期化を行うコードを記述する
    //////////////////////////////////////

    //ルートシグネチャを作成
    RootSignature rootSignature;
    InitRootSignature(rootSignature);

    //三角形ポリゴンを定義
    TrianglePolygon triangle;
    triangle.Init(rootSignature);

    // step-1 定数バッファを作成
    ConstantBuffer cb;
    cb.Init(sizeof(Matrix));
    // step-2 ディスクリプタヒープを作成
    DescriptorHeap ds;
    ds.RegistConstantBuffer(0, cb);
    ds.Commit();
    //////////////////////////////////////
    // 初期化を行うコードを書くのはここまで！！！
    //////////////////////////////////////
    auto& renderContext = g_graphicsEngine->GetRenderContext();
    int s = 0;
    // ここからゲームループ
    while (DispatchWindowMessage())
    {
        //フレーム開始
        g_engine->BeginFrame();

        //////////////////////////////////////
        //ここから絵を描くコードを記述する
        //////////////////////////////////////

        //ルートシグネチャを設定
        renderContext.SetRootSignature(rootSignature);

        // step-3 ワールド行列を作成
        Matrix mWorld0;
        Matrix mWorld1;
        float x = sin((s++ % 10000) / 1000.0f * 1 * M_PI);
        float y = cos((s++ % 10000) / 1000.0f * 1 * M_PI);
        float z = tan((s++ % 10000) / 1000.0f * 1 * M_PI);
        mWorld1.MakeTranslation(x, y, 0.0f);
        mWorld0.MakeRotationZ(z);
        mWorld0.Multiply(mWorld0, mWorld1);
        // step-4 ワールド行列をグラフィックメモリにコピー
        cb.CopyToVRAM(mWorld0);
        // step-5 ディスクリプタヒープを設定
        renderContext.SetDescriptorHeap(ds);
        //三角形をドロー
        triangle.Draw(renderContext);

        /// //////////////////////////////////////
        //絵を描くコードを書くのはここまで！！！
        //////////////////////////////////////
        //フレーム終了
        g_engine->EndFrame();
    }
    return 0;
}

//ルートシグネチャの初期化
void InitRootSignature(RootSignature& rs)
{
    rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}