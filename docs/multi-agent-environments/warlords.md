
# Warlords

```{figure} ../_static/videos/multi-agent-environments/warlords.gif
:width: 140px
:name: warlords
```

This environment is part of the <a href='..'>Atari environments</a>. Please read that page first for general information.

| Import               | `from pettingzoo.atari import warlords_v3`               |
|----------------------|----------------------------------------------------------|
| Actions              | Discrete                                                 |
| Parallel API         | Yes                                                      |
| Manual Control       | No                                                       |
| Agents               | `agents= ['first_0', 'second_0', 'third_0', 'fourth_0']` |
| Agents               | 4                                                        |
| Action Shape         | (1,)                                                     |
| Action Values        | [0,5]                                                    |
| Observation Shape    | (210, 160, 3)                                            |
| Observation Values   | (0,255)                                                  |

Four player last man standing! Defend your fortress from the ball and hit it towards your opponents. When your fortress falls, you receive -1 reward and are done. If you are the last player standing, you receive +1 reward.

[Official wizard_of_wor manual](https://atariage.com/manual_html_page.php?SoftwareLabelID=598)

## Environment parameters

Environment parameters are common to all Atari environments and are described in the [base multi-agent environment documentation](../multi-agent-environments).

## Action Space (Minimal)

In any given turn, an agent can choose from one of 6 actions.

| Action | Behavior     |
|:------:|--------------|
|   0    | No operation |
|   1    | Fire         |
|   2    | Move up      |
|   3    | Move right   |
|   4    | Move left    |
|   5    | Move down    |

## Version History

* v3: Minimal Action Space (1.18.0)
* v2: Breaking changes to entire API (1.4.0)
* v1: Fixes to how all environments handle premature death (1.3.0)
* v0: Initial versions release (1.0.0)
