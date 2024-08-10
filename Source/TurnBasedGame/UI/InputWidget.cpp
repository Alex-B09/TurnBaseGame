// Fill out your copyright notice in the Description page of Project Settings.


#include "InputWidget.h"

FGameplayTag UInputWidget::OnInputAction()
{
    FGameplayTag tag;
    OnInputAction(tag);
    return tag;
}