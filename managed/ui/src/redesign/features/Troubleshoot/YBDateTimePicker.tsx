import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import TextField from '@material-ui/core/TextField';
import clsx from 'clsx';

const useStyles = makeStyles((theme) => ({
  container: {
    display: 'flex',
    flexWrap: 'wrap'
  },
  textField: {
    marginLeft: theme.spacing(1),
    marginRight: theme.spacing(1),
    width: 200
  }
}));

interface YBDateTimePickerProps {
  dateTimeValue?: string;
  dateTimeLabel: string;
  className?: any;
  onChange?: (selectedValue: any) => void;
  disabled?: boolean;
}

export const YBDateTimePicker = ({
  dateTimeValue,
  dateTimeLabel,
  onChange,
  className,
  disabled = false
}: YBDateTimePickerProps) => {
  const classes = useStyles();

  return (
    <form className={classes.container} noValidate>
      <TextField
        id="datetime-local"
        label={dateTimeLabel ?? 'Next appointment'}
        type="datetime-local"
        defaultValue={dateTimeValue ?? '2017-05-24T10:30'}
        onChange={onChange}
        className={clsx(classes.textField, className)}
        InputLabelProps={{
          shrink: true
        }}
        disabled={disabled}
      />
    </form>
  );
};
