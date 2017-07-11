/*-----------------------------------------
    Вспомогательный файл для RdsFunc.h
  -----------------------------------------*/

#ifdef _RDS_AUX_FUNC
  _RDS_AUX_FUNC(rdsServiceVersion,RDS_IV)
  _RDS_AUX_FUNC(rdsCallerThreadType,RDS_IV)
  _RDS_AUX_FUNC(rdsGetTopWindowBlock,RDS_BhV)
  _RDS_AUX_FUNC(rdsGetBlockDescription,RDS_IBhBd)
  _RDS_AUX_FUNC(rdsGetAppWindowHandle,RDS_HwV)
  _RDS_AUX_FUNC(rdsGetMainWindow,RDS_HwV)
  _RDS_AUX_FUNC(rdsGetFirstBlock,RDS_BhBhIBd)
  _RDS_AUX_FUNC(rdsGetNextBlock,RDS_BhBhIBd)
  _RDS_AUX_FUNC(rdsGetConnDescription,RDS_IChCd)
  _RDS_AUX_FUNC(rdsGetFirstConn,RDS_ChBhICd)
  _RDS_AUX_FUNC(rdsGetNextConn,RDS_ChChICd)
  _RDS_AUX_FUNC(rdsGetPointDescription,RDS_IChIPd)
  _RDS_AUX_FUNC(rdsGetSystemPath,RDS_SI)
  _RDS_AUX_FUNC(rdsRemoteControllerCall,RDS_IIS)
  _RDS_AUX_FUNC(rdsRemoteReply,RDS_VS)
  _RDS_AUX_FUNC(rdsReportTextLoadError,RDS_VS)
  _RDS_AUX_FUNC(rdsSetHintText,RDS_VS)
  _RDS_AUX_FUNC(rdsGetRemoteControllerName,RDS_SV)
  _RDS_AUX_FUNC(rdsWriteBlockData,RDS_BpVI)
  _RDS_AUX_FUNC(rdsReadBlockData,RDS_BpVI)
  _RDS_AUX_FUNC(rdsWriteBlockDataText,RDS_VSB)
  _RDS_AUX_FUNC(rdsSetDebugText,RDS_VSB)
  _RDS_AUX_FUNC(rdsXGSetPenStyle,RDS_VIIICrI)
  _RDS_AUX_FUNC(rdsXGSetBrushStyle,RDS_VIICr)
  _RDS_AUX_FUNC(rdsXGSetFont,RDS_VISICrIIBBBB)
  _RDS_AUX_FUNC(rdsWriteFontText,RDS_VISICrIIBBBB)
  _RDS_AUX_FUNC(rdsXGSetLogFont,RDS_VpLfCr)
  _RDS_AUX_FUNC(rdsXGGetTextSize,RDS_VSpIpI)
  _RDS_AUX_FUNC(rdsXGRectangle,RDS_VIIII)
  _RDS_AUX_FUNC(rdsXGInvertRect,RDS_VIIII)
  _RDS_AUX_FUNC(rdsXGEllipse,RDS_VIIII)
  _RDS_AUX_FUNC(rdsXGArc,RDS_VIIIIIIII)
  _RDS_AUX_FUNC(rdsXGPie,RDS_VIIIIIIII)
  _RDS_AUX_FUNC(rdsXGChord,RDS_VIIIIIIII)
  _RDS_AUX_FUNC(rdsXGTextOut,RDS_VIIS)
  _RDS_AUX_FUNC(rdsXGTextRect,RDS_VIISpR)
  _RDS_AUX_FUNC(rdsSetBlockTimer,RDS_ThThDwDwB)
  _RDS_AUX_FUNC(rdsGetCustomColors,RDS_pCrV)
  _RDS_AUX_FUNC(rdsCallColorDialog,RDS_BpCr)
  _RDS_AUX_FUNC(rdsGetEditorParameters,RDS_BBhEd)
  _RDS_AUX_FUNC(rdsGetBlockTimerDescr,RDS_BThTd)
  _RDS_AUX_FUNC(rdsDeleteBlockTimer,RDS_VTh)
  _RDS_AUX_FUNC(rdsStopBlockTimer,RDS_VTh)
  _RDS_AUX_FUNC(rdsRestartBlockTimer,RDS_VThDw)
  _RDS_AUX_FUNC(rdsXGMoveTo,RDS_VII)
  _RDS_AUX_FUNC(rdsXGLineTo,RDS_VII)
  _RDS_AUX_FUNC(rdsGetTextWord,RDS_SSpSpCB)
  _RDS_AUX_FUNC(rdsWriteLineStyleText,RDS_VIB)
  _RDS_AUX_FUNC(rdsWriteWordValueText,RDS_VSI)
  _RDS_AUX_FUNC(rdsWriteColorText,RDS_VSCrB)
  _RDS_AUX_FUNC(rdsReadColorText,RDS_CrSpS)
  _RDS_AUX_FUNC(rdsReadFontText,RDS_BSpSSIpIpCrpIpIpBpBpBpB)
  _RDS_AUX_FUNC(rdsXGGetVisibleRect,RDS_VpR)
  _RDS_AUX_FUNC(rdsXGSetClipRect,RDS_VpR)
  _RDS_AUX_FUNC(rdsWriteWordStringText,RDS_VSS)
  _RDS_AUX_FUNC(rdsXGDrawBlockPicture,RDS_VIIDDDB)
  _RDS_AUX_FUNC(rdsXGTriangle,RDS_VIIIIII)
  _RDS_AUX_FUNC(rdsXGPolygon,RDS_VpPI)
  _RDS_AUX_FUNC(rdsXGFontSizeToHeight,RDS_II)
  _RDS_AUX_FUNC(rdsCheckBlockFunctionSupport,RDS_BBhI)
  _RDS_AUX_FUNC(rdsCallBlockFunction,RDS_IBhIpV)
  _RDS_AUX_FUNC(rdsBroadcastFunctionCallsEx,RDS_IBhIpVDw)
  _RDS_AUX_FUNC(rdsBroadcastFunctionCalls,RDS_VBhIpVB)
  _RDS_AUX_FUNC(rdsGetConnAppearance,RDS_VChCa)
  _RDS_AUX_FUNC(rdsSetConnAppearance,RDS_VChCa)
  _RDS_AUX_FUNC(rdsGetConnStyleAppearance,RDS_BSCa)
  _RDS_AUX_FUNC(rdsSystemInEditMode,RDS_BV)
  _RDS_AUX_FUNC(rdsGetBlockLink,RDS_ChBhChBBPd)
  _RDS_AUX_FUNC(rdsCreateDynamicVar,RDS_pVISSBS)
  _RDS_AUX_FUNC(rdsCreateAndSubscribeDV,RDS_DvISSBS)
  _RDS_AUX_FUNC(rdsDeleteDynamicVar,RDS_BIS)
  _RDS_AUX_FUNC(rdsSubscribeToDynamicVar,RDS_DvISSB)
  _RDS_AUX_FUNC(rdsUnsubscribeFromDynamicVar,RDS_VDv)
  _RDS_AUX_FUNC(rdsGetChildBlockByName,RDS_BhBhSBd)
  _RDS_AUX_FUNC(rdsGetIOBlockByVarName,RDS_BhBhSBd)
  _RDS_AUX_FUNC(rdsGetRootSystem,RDS_BhBd)
  _RDS_AUX_FUNC(rdsGetIOBlockLink,RDS_ChBhChPd)
  _RDS_AUX_FUNC(rdsRefreshBlockWindows,RDS_VBhB)
  _RDS_AUX_FUNC(rdsCountBlocks,RDS_IBhIB)
  _RDS_AUX_FUNC(rdsSetBlockComment,RDS_VBhS)
  _RDS_AUX_FUNC(rdsSetModifiedFlag,RDS_VB)
  _RDS_AUX_FUNC(rdsOpenSystemWindow,RDS_VBh)
  _RDS_AUX_FUNC(rdsCloseSystemWindow,RDS_VBh)
  _RDS_AUX_FUNC(rdsScrollWindowToBlock,RDS_VBh)
  _RDS_AUX_FUNC(rdsAltConnAppearanceOp,RDS_IChIICa)
  _RDS_AUX_FUNC(rdsGetLayerConfigName,RDS_SBhI)
  _RDS_AUX_FUNC(rdsGetLayerName,RDS_SBhI)
  _RDS_AUX_FUNC(rdsSetCurLayerConfig,RDS_BBhI)
  _RDS_AUX_FUNC(rdsSetCurLayerConfigByName,RDS_BBhS)
  _RDS_AUX_FUNC(rdsEnableWindowRefresh,RDS_VBhBB)
  _RDS_AUX_FUNC(rdsStopCalc,RDS_VV)
  _RDS_AUX_FUNC(rdsGetVarArrayParams,RDS_pVpVpIpI)
  _RDS_AUX_FUNC(rdsGetVarArrayAccessData,RDS_BpVAd)
  _RDS_AUX_FUNC(rdsResizeVarArray,RDS_BpVIIBAd)
  _RDS_AUX_FUNC(rdsEnumBlocks,RDS_BhBhIBCbpV)
  _RDS_AUX_FUNC(rdsGetPictureObjectId,RDS_IIIB)
  _RDS_AUX_FUNC(rdsSaveSystemState,RDS_IBhIBCb2)
  _RDS_AUX_FUNC(rdsLoadSystemState,RDS_BICb2)
  _RDS_AUX_FUNC(rdsGetScreenCoords,RDS_BBhIIpIpI)
  _RDS_AUX_FUNC(rdsLockBlockData,RDS_VV)
  _RDS_AUX_FUNC(rdsUnlockBlockData,RDS_VV)
  _RDS_AUX_FUNC(rdsBlockDataSyncCall,RDS_ICb4pV)
  _RDS_AUX_FUNC(rdsRegisterFunction,RDS_IS)
  _RDS_AUX_FUNC(rdsActivateOutputConnections,RDS_VBhB)
  _RDS_AUX_FUNC(rdsSelectBlock,RDS_VBhBB)
  _RDS_AUX_FUNC(rdsCalcProcessIsRunning,RDS_BV)
  _RDS_AUX_FUNC(rdsEnumConnectedBlocks,RDS_BhBhDwCb1pV)
  _RDS_AUX_FUNC(rdsWriteHexText,RDS_VpVII)
  _RDS_AUX_FUNC(rdsReadHexText,RDS_ISpSpVI)
  _RDS_AUX_FUNC(rdsGetCmdParamCount,RDS_IV)
  _RDS_AUX_FUNC(rdsGetCmdParam,RDS_SIB)
  _RDS_AUX_FUNC(rdsFindCmdParam,RDS_ISB)
  _RDS_AUX_FUNC(rdsRegisterMenuItem,RDS_MhSDwIDwII)
  _RDS_AUX_FUNC(rdsEnableMenuItem,RDS_VMhBB)
  _RDS_AUX_FUNC(rdsUnregisterMenuItem,RDS_VMh)
  _RDS_AUX_FUNC(rdsFree,RDS_VpV)
  _RDS_AUX_FUNC(rdsAllocate,RDS_pVDw)
  _RDS_AUX_FUNC(rdsCreateFullBlockNameString,RDS_SBhpI)
  _RDS_AUX_FUNC(rdsBlockByFullName,RDS_BhSBd)
  _RDS_AUX_FUNC(rdsParentIsRoot,RDS_BBh)
  _RDS_AUX_FUNC(rdsHasRemoteController,RDS_BV)
  _RDS_AUX_FUNC(rdsDeleteBlock,RDS_VBh)
  _RDS_AUX_FUNC(rdsDeleteConnection,RDS_VCh)
  _RDS_AUX_FUNC(rdsCreateBlockFromFile,RDS_BhSBhIIBd)
  _RDS_AUX_FUNC(rdsDuplicateBlock,RDS_BhBhBhIIBd)
  _RDS_AUX_FUNC(rdsGetBlockVar,RDS_VhBhiVd)
  _RDS_AUX_FUNC(rdsFindBlockVar,RDS_VhBhSBVd)
  _RDS_AUX_FUNC(rdsCECreateEditor,RDS_HoV)
  _RDS_AUX_FUNC(rdsDeleteObject,RDS_VHo)
  _RDS_AUX_FUNC(rdsCECreateConnBus,RDS_ChHoBhIpI)
  _RDS_AUX_FUNC(rdsCEEditConnBus,RDS_BHoChI)
  _RDS_AUX_FUNC(rdsCEAddInternalPoint,RDS_IHoII)
  _RDS_AUX_FUNC(rdsCEAddBlockPoint,RDS_IHoBhSIIB)
  _RDS_AUX_FUNC(rdsCEAddBusPoint,RDS_IHoChSBIIB)
  _RDS_AUX_FUNC(rdsCEAddLine,RDS_IHoII)
  _RDS_AUX_FUNC(rdsCEAddBezier,RDS_IHoIIIIII)
  _RDS_AUX_FUNC(rdsGetBlockDimensions,RDS_BBhBrB)
  _RDS_AUX_FUNC(rdsBCLCreateList,RDS_HoBhDwB)
  _RDS_AUX_FUNC(rdsGetObjectArray,RDS_pVHoIIpI)
  _RDS_AUX_FUNC(rdsMoveBlock,RDS_VBhII)
  _RDS_AUX_FUNC(rdsSetSystemUpdate,RDS_VB)
  _RDS_AUX_FUNC(rdsdebugBlockInfo,RDS_VBhSS)
  _RDS_AUX_FUNC(rdsGetLineDescription,RDS_BChILdPdPd)
  _RDS_AUX_FUNC(rdsFindNextConnectedLine,RDS_IChIILdpI)
  _RDS_AUX_FUNC(rdsScrollWindowToRect,RDS_VBhIIIIB)
  _RDS_AUX_FUNC(rdsCheckRectVisibility,RDS_BBhIIII)
  _RDS_AUX_FUNC(rdsCalcProcessNeverStarted,RDS_BV)
  _RDS_AUX_FUNC(rdsCheckSystemWindow,RDS_BBh)
  _RDS_AUX_FUNC(rdscompSetModelFunction,RDS_VMhSS)
  _RDS_AUX_FUNC(rdsDeleteDVByLink,RDS_BDv)
  _RDS_AUX_FUNC(rdsCopyVarArray,RDS_BpVpV)
  _RDS_AUX_FUNC(rdsVSCreateEditor,RDS_HoV)
  _RDS_AUX_FUNC(rdsVSInstallStruct,RDS_BHopI)
  _RDS_AUX_FUNC(rdsVSAddVar,RDS_IHoISCSDwIS)
  _RDS_AUX_FUNC(rdsSetObjectStr,RDS_VHoIIS)
  _RDS_AUX_FUNC(rdsVSApplyToBlock,RDS_BHoBhpI)
  _RDS_AUX_FUNC(rdsVSAddAutoConn,RDS_IHoSS)
  _RDS_AUX_FUNC(rdsCommandObjectEx,RDS_BHoIIpI)
  _RDS_AUX_FUNC(rdsCommandObject,RDS_BHoI)
  _RDS_AUX_FUNC(rdsGetVarField,RDS_VhVhiVd)
  _RDS_AUX_FUNC(rdsFindStructVar,RDS_VhSVd)
  _RDS_AUX_FUNC(rdsVSGetVarDescription,RDS_BHoIVd)
  _RDS_AUX_FUNC(rdsVSExecuteEditor,RDS_BHoBDwIS)
  _RDS_AUX_FUNC(rdsGetObjectInt,RDS_IHoII)
  _RDS_AUX_FUNC(rdsGetObjectStr,RDS_SHoII)
  _RDS_AUX_FUNC(rdsSetObjectInt,RDS_VHoIII)
  _RDS_AUX_FUNC(rdsCreateVarDescriptionString,RDS_SVhBIpI)
  _RDS_AUX_FUNC(rdsVSAddVarByDescr,RDS_IHoIS)
  _RDS_AUX_FUNC(rdsVSCreateByDescr,RDS_BHoS)
  _RDS_AUX_FUNC(rdsVSAddTypeRename,RDS_IHoSS)
  _RDS_AUX_FUNC(rdscompGetModelData,RDS_MdChI)
  _RDS_AUX_FUNC(rdsApplicationIsActive,RDS_BV)
  _RDS_AUX_FUNC(rdsRegisterWindow,RDS_BHwSHbHiS)
  _RDS_AUX_FUNC(rdsUnregisterWindow,RDS_VHw)
  _RDS_AUX_FUNC(rdsRegWinActivateNotify,RDS_VHw)
  _RDS_AUX_FUNC(rdsVSFindAutoConn,RDS_SHoS)
  _RDS_AUX_FUNC(rdscompGetModelBlock,RDS_BhMhIBd)
  _RDS_AUX_FUNC(rdsVSAddVarRename,RDS_IHoSS)
  _RDS_AUX_FUNC(rdsGetStructVar,RDS_VhIVd)
  _RDS_AUX_FUNC(rdsGetSystemInt,RDS_II)
  _RDS_AUX_FUNC(rdsVSUsesStructType,RDS_BHoS)
  _RDS_AUX_FUNC(rdsVarUsesStructType,RDS_BVhS)
  _RDS_AUX_FUNC(rdscompRenameModel,RDS_BChSSB)
  _RDS_AUX_FUNC(rdscompGetModelDataByName,RDS_MdChS)
  _RDS_AUX_FUNC(rdscompCompileModel,RDS_BMhB)
  _RDS_AUX_FUNC(rdscompReturnModelName,RDS_VS)
  _RDS_AUX_FUNC(rdsGetFullFilePath,RDS_SSSpI)
  _RDS_AUX_FUNC(rdsGetRelFilePath,RDS_SSSpI)
  _RDS_AUX_FUNC(rdsExecutePrintDialog,RDS_VBh)
  _RDS_AUX_FUNC(rdsChangeRegWinTitle,RDS_VHwS)
  _RDS_AUX_FUNC(rdsBCLAddBlock,RDS_IHoBhB)
  _RDS_AUX_FUNC(rdsBCLAddConn,RDS_IHoChB)
  _RDS_AUX_FUNC(rdsBCLExecuteGroupSetDialog,RDS_VHoDwSB)
  _RDS_AUX_FUNC(rdsIsRoot,RDS_BBh)
  _RDS_AUX_FUNC(rdsMessageBox,RDS_ISSI)
  _RDS_AUX_FUNC(rdscompReturnModelNameLabel,RDS_VS)
  _RDS_AUX_FUNC(rdscompSetAltModelName,RDS_VMhS)
  _RDS_AUX_FUNC(rdsSTRCreateTextReader,RDS_HoB)
  _RDS_AUX_FUNC(rdsSTRAddKeyword,RDS_IHoSI)
  _RDS_AUX_FUNC(rdsSTRAddKeywordsArray,RDS_BHopSII)
  _RDS_AUX_FUNC(rdsSTRGetWord,RDS_IHopSpSpCB)
  _RDS_AUX_FUNC(rdsWriteWordDoubleText,RDS_VSD)
  _RDS_AUX_FUNC(rdsReadLineStyleText,RDS_ISpS)
  _RDS_AUX_FUNC(rdsSetBlockVarDefValueStr,RDS_BBhIS)
  _RDS_AUX_FUNC(rdsGetBlockVarDefValueStr,RDS_SBhIpI)
  _RDS_AUX_FUNC(rdsRunWithoutEvents,RDS_BB)
  _RDS_AUX_FUNC(rdsXGFillRect,RDS_VIIII)
  _RDS_AUX_FUNC(rdsXGPolyline,RDS_VpPI)
  _RDS_AUX_FUNC(rdsXGRoundRect,RDS_VIIIIII)
  _RDS_AUX_FUNC(rdsSetPointPosition,RDS_BChIIIDw)
  _RDS_AUX_FUNC(rdsGetParentBlock,RDS_BhBhBd)
  _RDS_AUX_FUNC(rdsRenameBlock,RDS_BBhSBd)
  _RDS_AUX_FUNC(rdsVSGetVarDefValueStr,RDS_SHoIpI)
  _RDS_AUX_FUNC(rdsSetZoomPercent,RDS_VBhIII)
  _RDS_AUX_FUNC(rdsChangeMenuItem,RDS_BMhSDwIDwII)
  _RDS_AUX_FUNC(rdsResetSystemState,RDS_VBh);
  _RDS_AUX_FUNC(rdsINICreateTextHolder,RDS_HoB)
  _RDS_AUX_FUNC(rdsSetObjectDouble,RDS_VHoIID)
  _RDS_AUX_FUNC(rdsGetObjectDouble,RDS_DHoII)
  _RDS_AUX_FUNC(rdsINIOpenSection,RDS_BHoS)
  _RDS_AUX_FUNC(rdsINIWriteInt,RDS_VHoSI)
  _RDS_AUX_FUNC(rdsINIReadInt,RDS_IHoSI)
  _RDS_AUX_FUNC(rdsINIWriteDouble,RDS_VHoSD)
  _RDS_AUX_FUNC(rdsINIReadDouble,RDS_DHoSD)
  _RDS_AUX_FUNC(rdsINIWriteString,RDS_VHoSS)
  _RDS_AUX_FUNC(rdsINIReadString,RDS_SHoSSpI)
  _RDS_AUX_FUNC(rdscompAttachDifferentModel,RDS_VSS)
  _RDS_AUX_FUNC(rdscompSetBlockModel,RDS_IBhSSSS)
  _RDS_AUX_FUNC(rdscompGetBlockModelData,RDS_MdBh)
  _RDS_AUX_FUNC(rdscompOpenBlockModelEditor,RDS_IBh)
  _RDS_AUX_FUNC(rdsGetEditorWindowFlags,RDS_DwBh)
  _RDS_AUX_FUNC(rdsSetEditorWindowFlags,RDS_VBhDwDw)
  _RDS_AUX_FUNC(rdsEnumConnectedBlocksByVar,RDS_BhBhIDwCb1pV)
  _RDS_AUX_FUNC(rdsFORMCreate,RDS_HoBIIS)
  _RDS_AUX_FUNC(rdsFORMAddTab,RDS_VHoIS)
  _RDS_AUX_FUNC(rdsFORMAddEdit,RDS_VHoIIDwSI)
  _RDS_AUX_FUNC(rdsProcessText,RDS_SSIpI)
  _RDS_AUX_FUNC(rdsSetBlockSetupFuncName,RDS_VBhS)
  _RDS_AUX_FUNC(rdsDynStrCat,RDS_SSSB)
  _RDS_AUX_FUNC(rdsIsValidVarName,RDS_BSI)
  _RDS_AUX_FUNC(rdsRegisterFuncProvider,RDS_IIB)
  _RDS_AUX_FUNC(rdsUnregisterFuncProvider,RDS_BI)
  _RDS_AUX_FUNC(rdsSubscribeToFuncProvider,RDS_BFl)
  _RDS_AUX_FUNC(rdsUnsubscribeFromFuncProvider,RDS_VI)
  _RDS_AUX_FUNC(rdsStartCalc,RDS_VV)
  _RDS_AUX_FUNC(rdsFORMShowModalEx,RDS_BHoCb3)
  _RDS_AUX_FUNC(rdsGetHugeDouble,RDS_BpD)
  _RDS_AUX_FUNC(rdsSetBlockFlags,RDS_VBhDwDw)
  _RDS_AUX_FUNC(rdsSetBlockVarFlags,RDS_BBhIDwDw)
  _RDS_AUX_FUNC(rdsShowBlockPanelTab,RDS_VIS)
  _RDS_AUX_FUNC(rdsSetBlockAltNameText,RDS_VBhSI)
  _RDS_AUX_FUNC(rdsExecuteCommand,RDS_VIDwSS)
  _RDS_AUX_FUNC(rdsSetSystemWindowRect,RDS_VBhIIIIB)
  _RDS_AUX_FUNC(rdsRegisterContextMenuItem,RDS_MhSII)
  _RDS_AUX_FUNC(rdsEnableCommandQueue,RDS_VB)
  _RDS_AUX_FUNC(rdsSetSystemWindowCaption,RDS_VBhSB)
  _RDS_AUX_FUNC(rdsCopyRuntimeType,RDS_BpVpV)
  _RDS_AUX_FUNC(rdsClearRuntimeType,RDS_VpV)
  _RDS_AUX_FUNC(rdsPBARCreate,RDS_HoIS)
  _RDS_AUX_FUNC(rdsInputString,RDS_SSSSI)
  _RDS_AUX_FUNC(rdsUnlockAndCall,RDS_BCb4pVI)
  _RDS_AUX_FUNC(rdsGetRemoteControllerString,RDS_SI)
  _RDS_AUX_FUNC(rdsGetRuntimeTypeData,RDS_pVpVpS)
  _RDS_AUX_FUNC(rdsShowMainWindow,RDS_VB)
  _RDS_AUX_FUNC(rdsMainWindowVisible,RDS_BV)
  _RDS_AUX_FUNC(rdsAdditionalContextMenuItem,RDS_VSBII)
  _RDS_AUX_FUNC(rdsGetBlockFlags,RDS_DwBh)
  _RDS_AUX_FUNC(rdsSetRuntimeType,RDS_pVpVS)
  _RDS_AUX_FUNC(rdsBringAppToFront,RDS_VV)
  _RDS_AUX_FUNC(rdsBlockModalWinOpen,RDS_VBh)
  _RDS_AUX_FUNC(rdsBlockModalWinClose,RDS_VBh)
  _RDS_AUX_FUNC(rdsModalWindowMustClose,RDS_BV)
  _RDS_AUX_FUNC(rdsExecMenuItem,RDS_VBhII)
  _RDS_AUX_FUNC(rdsGetLayerId,RDS_IBhS)
  _RDS_AUX_FUNC(rdsAddLayer,RDS_IBhS)
  _RDS_AUX_FUNC(rdsSetBlockLayer,RDS_BBhI)
  _RDS_AUX_FUNC(rdsGetAppInstance,RDS_HiV)
  _RDS_AUX_FUNC(rdsModalWindowExists,RDS_BV)
  _RDS_AUX_FUNC(rdsEnumDynVarSubscribers,RDS_BhDvCb5pV)
  _RDS_AUX_FUNC(rdsNotifyDynVarSubscribers,RDS_VDv)
  _RDS_AUX_FUNC(rdsCancelPaste,RDS_VV)
  _RDS_AUX_FUNC(rdsdebugLogString,RDS_BSSB)
  _RDS_AUX_FUNC(rdsGetObjectDoubleP,RDS_VHoIIpD)
  _RDS_AUX_FUNC(rdsINIReadDoubleP,RDS_VHoSDpD)
  _RDS_AUX_FUNC(rdsNetServer,RDS_IISB)
  _RDS_AUX_FUNC(rdsNetConnect,RDS_ISISB)
  _RDS_AUX_FUNC(rdsNetCloseConnection,RDS_VI)
  _RDS_AUX_FUNC(rdsNetBroadcastData,RDS_BIDwISpVDw)
  _RDS_AUX_FUNC(rdsNetSendData,RDS_BIDwISpVDwNsNb)
  _RDS_AUX_FUNC(rdsBlockVarToMem,RDS_pVBhIBpI)
  _RDS_AUX_FUNC(rdsBlockVarFromMem,RDS_BBhIpVI)
  _RDS_AUX_FUNC(rdsXGDrawStdIcon,RDS_VIIDw)
  _RDS_AUX_FUNC(rdsXGGetStdIconSize,RDS_BDwpIpI)
  _RDS_AUX_FUNC(rdsBlockMessageBox,RDS_IBhSSI)
  _RDS_AUX_FUNC(rdsFORMShowModalServ,RDS_BHoCb6)
  _RDS_AUX_FUNC(rdsCallBlockFunctionDelayed,RDS_VBhIpVDw)
  _RDS_AUX_FUNC(rdsBroadcastFuncCallsDelayed,RDS_VBhIpVDwDw)
  _RDS_AUX_FUNC(rdsFORMEnableSidePanel,RDS_VHoII)
  _RDS_AUX_FUNC(rdsSetExclusiveCalc,RDS_BBhB)
  _RDS_AUX_FUNC(rdsListVarTypes,RDS_SDwS)
  _RDS_AUX_FUNC(rdsCreateVarTypeText,RDS_SVh)
  _RDS_AUX_FUNC(rdsVSAddVarByTypeText,RDS_IHoISSDwS)
  _RDS_AUX_FUNC(rdsCopyVarGeneral,RDS_BVhpVVhpV)
  _RDS_AUX_FUNC(rdsSetBlockVarDefValueByCur,RDS_BBhI)
  _RDS_AUX_FUNC(rdsGetBlockVarBase,RDS_pVBhIpI)
  _RDS_AUX_FUNC(rdsVSCreateFromBlock,RDS_VHoBhB)
  _RDS_AUX_FUNC(rdsAtoI,RDS_ISpI)
  _RDS_AUX_FUNC(rdsItoA,RDS_SIII)
  _RDS_AUX_FUNC(rdsAtoD,RDS_VSpD)
  _RDS_AUX_FUNC(rdsDtoA,RDS_SDIpI)
  _RDS_AUX_FUNC(rdsSetBlockModel,RDS_BBhSS)
  _RDS_AUX_FUNC(rdsSetSystemInt,RDS_VII)
  _RDS_AUX_FUNC(rdsBEUCreate,RDS_HoBh)
  _RDS_AUX_FUNC(rdsXGSetPixel,RDS_VII)
  _RDS_AUX_FUNC(rdsGetMouseObjectId,RDS_IMd)
  _RDS_AUX_FUNC(rdsOpenSystemWindowEx,RDS_VBhBIIII)
  _RDS_AUX_FUNC(rdsSetSystemWindowBounds,RDS_VBhBIIII)
  _RDS_AUX_FUNC(rdsVarArrayIndexCheck,RDS_BpVIIDwSS)
  _RDS_AUX_FUNC(rdsBadSystemTime,RDS_BV)
  _RDS_AUX_FUNC(rdsExecutesRemoteOpsSet,RDS_VSB)
  _RDS_AUX_FUNC(rdsSetLayerPosition,RDS_BBhIIII)
  _RDS_AUX_FUNC(rdsGetLayerParams,RDS_BBhIIpBpBpB)
  _RDS_AUX_FUNC(rdsSetLayerParams,RDS_BBhIIBBB)
  _RDS_AUX_FUNC(rdsSetBlockRect,RDS_VBhIIII)
  _RDS_AUX_FUNC(rdsGetConnDimensions,RDS_BChBrB)
  _RDS_AUX_FUNC(rdsSetConnLayer,RDS_BChI)
  _RDS_AUX_FUNC(rdsUpdateExtIdsRange,RDS_VV)
  _RDS_AUX_FUNC(rdsBlockOrConnByExtId,RDS_BBhDwFbei)
  _RDS_AUX_FUNC(rdsPANCreate,RDS_HoIIIIIIS)
  _RDS_AUX_FUNC(rdsPANGetDescr,RDS_BHoPnd)
  _RDS_AUX_FUNC(rdsGetEditorFont,RDS_BBhIpLfDwpI)
  _RDS_AUX_FUNC(rdsXGSetFontByParStr,RDS_VpFsD)
  _RDS_AUX_FUNC(rdsFontTextToStruct,RDS_BSpSpFs)
  _RDS_AUX_FUNC(rdsStructToFontText,RDS_SpFspI)
  _RDS_AUX_FUNC(rdsAddToDynStr,RDS_VpSSB)
  _RDS_AUX_FUNC(rdsForceBlockRedraw,RDS_VBh)
  _RDS_AUX_FUNC(rdsRegisterContextMenuItemEx,RDS_MhSDwII)
  _RDS_AUX_FUNC(rdsSetMenuItemOptions,RDS_VMhDw)
  _RDS_AUX_FUNC(rdsAdditionalContextMenuItemEx,RDS_VSDwII)
  _RDS_AUX_FUNC(rdsQueueCallBlockFunction,RDS_VBhIpVDwDw)
  _RDS_AUX_FUNC(rdsGetBlockDimensionsEx,RDS_BBhBrDw)
  _RDS_AUX_FUNC(rdsDeleteSystemState,RDS_VI)
  _RDS_AUX_FUNC(rdsVSSetVarFlags,RDS_VHoIDwDw)
  _RDS_AUX_FUNC(rdsMakeUniqueBlockName,RDS_SBhS)
  _RDS_AUX_FUNC(rdsCSVCreate,RDS_HoV)
  _RDS_AUX_FUNC(rdsCSVSetItem,RDS_VHoIIS)
  _RDS_AUX_FUNC(rdsCSVGetItem,RDS_SHoII)
  _RDS_AUX_FUNC(rdsStringReplace,RDS_SSpSpSIDw)
  _RDS_AUX_FUNC(rdsDynStrCopy,RDS_SS)
  _RDS_AUX_FUNC(rdsCallFileDialog,RDS_SSDwSSS)
  _RDS_AUX_FUNC(rdsCallDirDialog,RDS_SSSB)
  _RDS_AUX_FUNC(rdsTransformFileName,RDS_SSDwSpI)
  _RDS_AUX_FUNC(rdsTMPCreateFileSet,RDS_IV)
  _RDS_AUX_FUNC(rdsTMPDeleteFileSet,RDS_VI)
  _RDS_AUX_FUNC(rdsTMPCreateEmptyFile,RDS_SIS)
  _RDS_AUX_FUNC(rdsTMPDeleteFile,RDS_VS)
  _RDS_AUX_FUNC(rdsTMPRememberFileName,RDS_SIS)
  _RDS_AUX_FUNC(rdsGetLayerIdInConfig,RDS_IBhII)
  _RDS_AUX_FUNC(rdsGetTextWordDyn,RDS_SSpSpCB)
  _RDS_AUX_FUNC(rdsCEAddChannel,RDS_BHoSCSI)
  _RDS_AUX_FUNC(rdsReportBMPConvertInfo,RDS_VS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsReportToolInfo,RDS_VS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSetToolTimer,RDS_VDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsExecTool,RDS_IDwSSIS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSaveSystemBitmap,RDS_BS1d) // **** REV2 ****
  _RDS_AUX_FUNC(rdsPictureObjectDimensions,RDS_BIDwBr) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSuspendCalc,RDS_BBhB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsServiceVersionDate,RDS_DwV) // **** REV2 ****
  _RDS_AUX_FUNC(rdsReportVersion,RDS_VIDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsVSCompare,RDS_BHoHoDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetBlockVarTypeString,RDS_SBh) // **** REV2 ****
  _RDS_AUX_FUNC(rdsBLOBStartWrite,RDS_VIIB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsBLOBWrite,RDS_BpVI) // **** REV2 ****
  _RDS_AUX_FUNC(rdsBLOBEndWrite,RDS_VV) // **** REV2 ****
  _RDS_AUX_FUNC(rdsBLOBStartRead,RDS_BII) // **** REV2 ****
  _RDS_AUX_FUNC(rdsBLOBRead,RDS_BpVI) // **** REV2 ****
  _RDS_AUX_FUNC(rdsBLOBEndRead,RDS_VV) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetIOBlockByVarNum,RDS_BhBhIBd) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSetEditorWorkspace,RDS_BBhII) // **** REV2 ****
  _RDS_AUX_FUNC(rdsShowHelp,RDS_ISSB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsShowHelpFile,RDS_ISSB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsRegisterConnExtParamName,RDS_IS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetConnExtParamsCount,RDS_ICh) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetConnExtParamByNum,RDS_IChIpSpIpIpDpS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetConnExtParamById,RDS_IChIpIpDpS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSetConnExtParamById,RDS_BChIIIDS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsConnSetupMenuName,RDS_VS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsEnumDynVars,RDS_IBhDwCb7pV) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetConnExtParamByNumEx,RDS_IChIpSpIpIpDpSpDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetConnExtParamByIdEx,RDS_IChIpIpDpSpDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSetConnExtParamByIdEx,RDS_BChIIIDSDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsCallSaveAsDialog,RDS_BV) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSetObjectPtr,RDS_VHoISpV) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetObjectPtr,RDS_pVHoIpS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsExchangeVarArrays,RDS_BpVpV) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetVarArrayElementType,RDS_IpV) // **** REV2 ****
  _RDS_AUX_FUNC(rdsCopySelectedToMem,RDS_pVBhpIDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSelectConn,RDS_VChBB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSelectAll,RDS_VBhBB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsPasteFromMem,RDS_BBhpVIDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsDeleteSelected,RDS_VBhB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsCEUCreate,RDS_HoCh) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSubscribeToContentChange,RDS_VB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsUnsubscribeFromContentChange,RDS_VV) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSetSysModifiedFlag,RDS_VBhB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsTMPForgetFileName,RDS_BIS) // **** REV2 ****
  _RDS_AUX_FUNC(rdscompGetModelModuleData,RDS_CdCh) // **** REV2 ****
  _RDS_AUX_FUNC(rdscompIsEditorOpen,RDS_BMh) // **** REV2 ****
  _RDS_AUX_FUNC(rdsExclusiveFileOp,RDS_VSDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsInfoMessage,RDS_VSS) // **** REV2 ****
  _RDS_AUX_FUNC(rdscompCompileAll,RDS_BB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsVSCreateAutoConnsByDescr,RDS_BHoS) // **** REV2 ****
  _RDS_AUX_FUNC(rdscompLibraryOperation,RDS_BLop) // **** REV2 ****
  _RDS_AUX_FUNC(rdsStoreTempGlobal,RDS_VSSISpVDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsReadTempGlobal,RDS_BSSpIpSppVpDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsDeleteConnExtParamById,RDS_BChI) // **** REV2 ****
  _RDS_AUX_FUNC(rdsExecToolExt,RDS_IDwSSS2) // **** REV2 ****
  _RDS_AUX_FUNC(rdsClearExecToolExtResults,RDS_VS2) // **** REV2 ****
  _RDS_AUX_FUNC(rdsCountSelectedObjects,RDS_BBhpIpI) // **** REV2 ****
  _RDS_AUX_FUNC(rdsFORMHelpButton,RDS_VHoDwSS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsRegisterUserNameSet,RDS_IS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsRegisterUserName,RDS_IIS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetRegisteredUserName,RDS_SII) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetBlockVarOffset,RDS_IBhIpI) // **** REV2 ****
  _RDS_AUX_FUNC(rdsRegisterGlobalIntVar,RDS_pIS) // **** REV2 ****
  _RDS_AUX_FUNC(rdscompForceModelNextCompile,RDS_VMhB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsDoInitialCalc,RDS_VBhDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsShiftBakFileChain,RDS_BSIS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetBlockTextRectParams,RDS_BBhpBtrpFs) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSetBlockTextRectParams,RDS_BBhpBtrpFsDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsINTCreate,RDS_HoB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSCEUCreate,RDS_HoBh) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSCEUSaveBlock,RDS_VHoBhDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSCEUSaveConn,RDS_VHoChDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsExecToolDelayed,RDS_VDwSSS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsLockDrawUpdates,RDS_BBhDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsXGDrawBitmap,RDS_VIIHb) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetEditorWinAndScroll,RDS_BBhpIpIpIpIpIpI) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGBLCreate,RDS_HoS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGBLBlockOp,RDS_VHoBhDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGBLGetBlock,RDS_BhHoI) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGBLBroadcastFunctionCalls,RDS_IHoIpVDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsXGDrawStdIconDC,RDS_VDcIIDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsInvalidateSystemWindows,RDS_VBhB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSaveBlockToFile,RDS_BBhSDwS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSetBlockOrder,RDS_BhIBh) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSetConnOrder,RDS_ChICh) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSetConnText,RDS_BChDwSIBII) // **** REV2 ****
  _RDS_AUX_FUNC(rdscompCompileBlockModelDelayed,RDS_VBhB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsFileExists,RDS_BSSpSpI) // **** REV2 ****
  _RDS_AUX_FUNC(rdsBlockStateToBuffer,RDS_pVBhpDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsBlockStateFromBuffer,RDS_BBhpVDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsCreateSharedMemPlain,RDS_SmISDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsCreateSharedMemVar,RDS_SmISS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsDeleteSharedMem,RDS_VSm) // **** REV2 ****
  _RDS_AUX_FUNC(rdsNotifySharedMemUsers,RDS_VSm) // **** REV2 ****
  _RDS_AUX_FUNC(rdsEnumSharedMemUsers,RDS_BhSmCb8pV) // **** REV2 ****
  _RDS_AUX_FUNC(rdsTranslateParameter,RDS_SSIISpI) // **** REV2 ****
  _RDS_AUX_FUNC(rdsTranslateText,RDS_SSIpI) // **** REV2 ****
  _RDS_AUX_FUNC(rdsSetConnTextEx,RDS_BChDwSIBIIB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetLayerConfigId,RDS_DwBhI) // **** REV2 ****
  _RDS_AUX_FUNC(rdsGetLayerConfigNumById,RDS_IBhDw) // **** REV2 ****
  _RDS_AUX_FUNC(rdsFORMTBLSetStr,RDS_VHoIIIS) // **** REV2 ****
  _RDS_AUX_FUNC(rdsFORMTBLGetStr,RDS_SHoIIIpB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsFORMTBLSetHeader,RDS_VHoIISDwI) // **** REV2 ****
  _RDS_AUX_FUNC(rdsFORMTBLSetInt,RDS_VHoIIII) // **** REV2 ****
  _RDS_AUX_FUNC(rdsFORMTBLGetInt,RDS_IHoIIIpB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsFORMTBLSetDouble,RDS_VHoIIID) // **** REV2 ****
  _RDS_AUX_FUNC(rdsFORMTBLGetDouble,RDS_DHoIIIpB) // **** REV2 ****
  _RDS_AUX_FUNC(rdsFORMTBLGetDoubleP,RDS_BHoIIIpD) // **** REV2 ****
  _RDS_AUX_FUNC(rdsFORMTBLMoveLine,RDS_BHoIII) // **** REV2 ****
#endif

