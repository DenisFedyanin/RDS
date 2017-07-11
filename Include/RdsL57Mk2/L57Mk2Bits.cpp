//---------------------------------------------------------------------------
// Детали встраиваемых функций для поддержки полуавтокомпилируемых блоков
// Версия 2
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Вычисление max / arg max
//---------------------------------------------------------------------------
#ifdef L57_ARGMAX_CALL
double l57_m_maxval=rdsbcppHugeDouble,l57_m_maxarg=rdsbcppHugeDouble;
double l57_min,l57_max,l57_step;
// Вычисление числа точек с начальным шагом
int l57_count=L57_AdjustDiapAndStep(l57_min,l57_max,l57_step,
                                    L57_M_MIN,L57_M_MAX,L57_M_STEP);
int l57_final=l57_count-1;
double l57_v_prev,l57_v,l57_v_next;
double l57_a_prev,l57_a,l57_a_next;

#if defined(L57_M_ENDSTEP) || defined(L57_M_DELTA)
  double l57_cstep;
#endif

#ifdef L57_M_ENDSTEP
  BOOL l57_endstepok=(L57_M_ENDSTEP!=rdsbcppHugeDouble && L57_M_ENDSTEP>0.0);
#else
  BOOL l57_endstepok=FALSE;
#endif

#ifdef L57_M_DELTA
  int l57_cycles_in_df;
  double l57_oldmaxval;
#endif

// Первая точка
l57_a=l57_v=rdsbcppHugeDouble;
l57_a_next=L57_ValueInDiap(l57_min,l57_max,l57_step,0,l57_count);
l57_v_next=L57_ARGMAX_CALL(l57_a_next);

// Главный цикл перебора (с начальным шагом)
for(int l57_index=0;l57_index<l57_count;l57_index++)
  { BOOL l57_pointready;
    double l57_carg,l57_cval;
    // Предыдущая, текущая и следующая точки
    l57_a_prev=l57_a; l57_v_prev=l57_v;
    l57_a=l57_a_next; l57_v=l57_v_next;
    if(l57_index==l57_final) // Правее точек нет
      l57_a_next=l57_v_next=rdsbcppHugeDouble;
    else
      { l57_a_next=L57_ValueInDiap(l57_min,l57_max,l57_step,l57_index+1,l57_count);
        l57_v_next=L57_ARGMAX_CALL(l57_a_next);
      }
    if(l57_v==rdsbcppHugeDouble) // Текущая точка не существует
      l57_pointready=FALSE;
    else
      l57_pointready=(l57_v_prev==rdsbcppHugeDouble || l57_v_prev<=l57_v) &&
                     (l57_v_next==rdsbcppHugeDouble || l57_v_next<=l57_v);
    if(!l57_pointready)
      continue;
    // Оба соседа текущей точки меньше ее
    l57_carg=l57_a; l57_cval=l57_v;
    #if defined(L57_M_ENDSTEP) || defined(L57_M_DELTA)
      // Уточнение
      l57_cstep=l57_step;
      #ifdef L57_M_DELTA
        l57_cycles_in_df=0;
      #endif
      for(int l57_totalitercount=0;l57_totalitercount<L57_M_MAXITER;l57_totalitercount++)
        { double l57_leftarg,l57_rightarg;
          BOOL l57_cand_ready=TRUE;
          // Проверяем условие конца уточнения
          #ifdef L57_M_ENDSTEP
            if(l57_cstep>L57_M_ENDSTEP)
              l57_cand_ready=FALSE;
          #endif
          #ifdef L57_M_DELTA
            if(l57_cycles_in_df<L57_M_ITERATIONS)
              { l57_cand_ready=FALSE;
                l57_oldmaxval=l57_cval;
              }
          #endif
          if(l57_cand_ready) // Условие окончания уточнения достигнуто
            break;
          // Уменьшаем шаг в два раза
          l57_cstep/=2;
          l57_leftarg=l57_carg-l57_cstep;
          l57_rightarg=l57_carg+l57_cstep;
          // Левая точка
          if(l57_leftarg>l57_min)
            { double l57_newval=L57_ARGMAX_CALL(l57_leftarg);
              if(l57_newval!=rdsbcppHugeDouble && l57_newval>l57_cval)
                { l57_cval=l57_newval; l57_carg=l57_leftarg; }
            }
          // Правая точка
          if(l57_rightarg<l57_max)
            { double l57_newval=L57_ARGMAX_CALL(l57_rightarg);
              if(l57_newval!=rdsbcppHugeDouble && l57_newval>l57_cval)
                { l57_cval=l57_newval; l57_carg=l57_rightarg; }
            }
          #ifdef L57_M_DELTA
            if(fabs(l57_cval-l57_oldmaxval)<L57_M_DELTA) // В трубке
              l57_cycles_in_df++;
            else // Вне трубки
              l57_cycles_in_df=0;
          #endif
        }
    #endif // #if defined(L57_M_ENDSTEP) || defined(L57_M_DELTA)
    // Уточненный (возможно) локальный максимум - в l57_carg,l57_cval
    if(l57_m_maxval==rdsbcppHugeDouble || l57_m_maxval<l57_cval)
      { l57_m_maxarg=l57_carg;
        l57_m_maxval=l57_cval;
      }
  }

#endif // #ifdef L57_ARGMAX_CALL
//---------------------------------------------------------------------------

