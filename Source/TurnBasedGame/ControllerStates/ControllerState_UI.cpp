// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerState_UI.h"

void UControllerState_UI::Setup(UInputWidget* widget)
{
    mWidget = widget;
}

void UControllerState_UI::OnMoveUp()
{
    mWidget->OnInputUp();
}

void UControllerState_UI::OnMoveDown()
{
    mWidget->OnInputDown();
}

void UControllerState_UI::OnMoveLeft()
{
    mWidget->OnInputLeft();
}

void UControllerState_UI::OnMoveRight()
{
    mWidget->OnInputRight();
}

void UControllerState_UI::OnAction()
{
    mWidget->OnInputAction();
    ActionSelectedEvent.Broadcast();
}

void UControllerState_UI::OnCancel()
{
    mWidget->OnInputCancel();
    CancelSelectedEvent.Broadcast();
}
